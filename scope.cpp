#include "scope.h"

namespace CODEGEN {
std::vector<IR::Vector<IR::Node> *> P4Scope::scope;
std::set<cstring> P4Scope::used_names;
std::map<cstring, std::map<int, std::set<cstring>>> P4Scope::lval_map;
std::map<cstring, std::map<int, std::set<cstring>>> P4Scope::lval_map_rw;

const IR::Type *P4Scope::ret_type = nullptr;
std::vector<IR::P4Control *> P4Scope::p4_ctrls;
std::set<const IR::P4Table *> P4Scope::callable_tables;
const IR::Type_Struct *P4Scope::sys_hdr;

// TODO: This should be set by the backend
std::set<cstring> P4Scope::not_initialized_structs = {
    "ingress_intrinsic_metadata_t",
    "ingress_intrinsic_metadata_from_parser_t",
    "ingress_intrinsic_metadata_for_deparser_t",
    "egress_intrinsic_metadata_t",
    "egress_intrinsic_metadata_from_parser_t",
    "egress_intrinsic_metadata_for_deparser_t",
    "egress_intrinsic_metadata_for_output_port_t",
};

void P4Scope::add_to_scope(const IR::Node *n) {
    auto l_scope = P4Scope::scope.back();

    l_scope->push_back(n);

    if (auto dv = n->to<IR::Declaration_Variable>()) {
        add_lval(dv->type, dv->name.name);
    }
}

void P4Scope::start_local_scope() {
    IR::Vector<IR::Node> *local_scope = new IR::Vector<IR::Node>();

    scope.push_back(local_scope);
}

void P4Scope::end_local_scope() {
    IR::Vector<IR::Node> *local_scope = scope.back();

    for (auto node : *local_scope) {
        if (auto decl = node->to<IR::Declaration_Variable>()) {
            delete_lval(decl->type, decl->name.name);
        } else if (auto param = node->to<IR::Parameter>()) {
            delete_lval(param->type, param->name.name);
        }
    }

    delete local_scope;
    scope.pop_back();
}

void add_compound_lvals(const IR::Type_StructLike *sl_type, cstring sl_name) {
    for (auto field : sl_type->fields) {
        std::stringstream ss;
        ss.str("");
        ss << sl_name << "." << field->name.name;
        cstring field_name(ss.str());
        P4Scope::add_lval(field->type, field_name);
    }
}

void delete_compound_lvals(const IR::Type_StructLike *sl_type,
                           cstring sl_name) {
    for (auto field : sl_type->fields) {
        std::stringstream ss;
        ss.str("");
        ss << sl_name << "." << field->name.name;
        cstring field_name(ss.str());
        P4Scope::delete_lval(field->type, field_name);
    }
}

void P4Scope::delete_lval(const IR::Type *tp, cstring name) {
    cstring type_key;
    int bit_bucket;

    if (auto tb = tp->to<IR::Type_Bits>()) {
        type_key = IR::Type_Bits::static_type_name();
        bit_bucket = tb->width_bits();
    } else if (auto tn = tp->to<IR::Type_Name>()) {
        auto tn_name = tn->path->name.name;
        if (tn_name == "packet_in") {
            return;
        }
        if (auto td = get_type_by_name(tn_name)) {
            if (auto tn_type = td->to<IR::Type_StructLike>()) {
                // width_bits should work here, do not know why not...
                type_key = tn_name;
                // does not work for some reason...
                // bit_bucket = P4Scope::compound_type[tn_name]->width_bits();
                bit_bucket = 1;
                delete_compound_lvals(tn_type, name);
            } else {
                BUG("Type %s not yet supported", td->node_type_name());
            }
        } else {
            printf("Type %s does not exist!\n", tn_name);
            return;
        }
    } else {
        BUG("Type %s not yet supported", tp->node_type_name());
    }
    lval_map[type_key][bit_bucket].erase(name);

    // delete values in the normal map
    if (lval_map[type_key][bit_bucket].size() == 0) {
        lval_map[type_key].erase(bit_bucket);
    }

    // delete values in the rw map
    if (lval_map_rw.count(type_key) != 0) {
        if (lval_map_rw[type_key].count(bit_bucket) != 0) {
            lval_map_rw[type_key][bit_bucket].erase(name);
            if (lval_map_rw[type_key][bit_bucket].size() == 0) {
                lval_map_rw[type_key].erase(bit_bucket);
            }
        }
    }
    // delete empty type entries
    if (lval_map[type_key].size() == 0) {
        lval_map.erase(type_key);
    }
    if (lval_map_rw[type_key].size() == 0) {
        lval_map_rw.erase(type_key);
    }
}

void P4Scope::add_lval(const IR::Type *tp, cstring name, bool read_only) {
    cstring type_key;
    int bit_bucket;

    if (auto tb = tp->to<IR::Type_Bits>()) {
        type_key = IR::Type_Bits::static_type_name();
        bit_bucket = tb->width_bits();
    } else if (auto tn = tp->to<IR::Type_Name>()) {
        auto tn_name = tn->path->name.name;
        if (tn_name == "packet_in") {
            return;
        }
        if (auto td = get_type_by_name(tn_name)) {
            if (auto tn_type = td->to<IR::Type_StructLike>()) {
                // width_bits should work here, do not know why not...
                type_key = tn_name;
                // does not work for some reason...
                // bit_bucket = P4Scope::compound_type[tn_name]->width_bits();
                bit_bucket = 1;
                add_compound_lvals(tn_type, name);
            } else {
                BUG("Type %s not yet supported", td->node_type_name());
            }
        } else {
            BUG("Type %s does not exist", tn_name);
        }
    } else {
        BUG("Type %s not yet supported", tp->node_type_name());
    }
    if (not read_only) {
        lval_map_rw[type_key][bit_bucket].insert(name);
    }
    lval_map[type_key][bit_bucket].insert(name);
}

std::set<cstring> P4Scope::get_candidate_lvals(const IR::Type *tp,
                                               bool must_write) {
    cstring type_key;
    int bit_bucket;

    if (auto tb = tp->to<IR::Type_Bits>()) {
        type_key = IR::Type_Bits::static_type_name();
        bit_bucket = tb->width_bits();
    } else if (auto tn = tp->to<IR::Type_Name>()) {
        auto tn_name = tn->path->name.name;
        if (auto td = get_type_by_name(tn_name)) {
            // bit_bucket = td->width_bits();
            bit_bucket = 1;
        } else {
            BUG("Type_name refers to unknown type %s", tn_name);
        }
    } else {
        BUG("Type %s not yet supported", tp->node_type_name());
    }

    std::map<cstring, std::map<int, std::set<cstring>>> lookup_map;

    if (must_write) {
        lookup_map = lval_map_rw;
    } else {
        lookup_map = lval_map;
    }

    if (lookup_map.count(type_key) == 0) {
        return {};
    }
    auto key_types = lookup_map[type_key];
    if (key_types.count(bit_bucket) == 0) {
        return {};
    }

    return key_types[bit_bucket];
}

bool P4Scope::check_lval(const IR::Type *tp, bool must_write) {
    std::set<cstring> candidates = get_candidate_lvals(tp, must_write);

    if (candidates.empty()) {
        return false;
    }
    return true;
}

cstring P4Scope::pick_lval(const IR::Type *tp, bool must_write) {
    std::set<cstring> candidates = get_candidate_lvals(tp, must_write);

    if (candidates.empty()) {
        BUG("Invalid Type Query, %s not found", tp->toString());
    }
    size_t idx = rand() % candidates.size();
    auto lval = std::begin(candidates);
    // 'advance' the iterator idx times
    std::advance(lval, idx);

    return *lval;
}

IR::Type_Bits *P4Scope::pick_declared_bit_type(bool must_write) {
    std::map<cstring, std::map<int, std::set<cstring>>> lookup_map;

    if (must_write) {
        lookup_map = lval_map_rw;
    } else {
        lookup_map = lval_map;
    }

    cstring bit_key = IR::Type_Bits::static_type_name();
    if (lookup_map.count(bit_key) == 0) {
        return nullptr;
    }
    auto key_types = lookup_map[bit_key];
    size_t idx = rand() % key_types.size();
    int bit_width = next(key_types.begin(), idx)->first;
    return new IR::Type_Bits(bit_width, false);
}

std::vector<const IR::Type_Declaration *>
P4Scope::get_filtered_decls(std::set<cstring> filter) {
    std::vector<const IR::Type_Declaration *> ret;
    for (auto sub_scope : scope) {
        for (auto node : *sub_scope) {
            cstring name = node->node_type_name();
            if (filter.find(name) == filter.end()) {
                if (auto decl = node->to<IR::Type_Declaration>()) {
                    ret.push_back(decl);
                }
            }
        }
    }
    return ret;
}

std::set<const IR::P4Table *> *P4Scope::get_callable_tables() {
    return &callable_tables;
}

const IR::Type_Declaration *P4Scope::get_type_by_name(cstring name) {
    for (auto sub_scope : scope) {
        for (auto node : *sub_scope) {
            if (auto decl = node->to<IR::Type_Declaration>()) {
                if (decl->name.name == name) {
                    return decl;
                }
            }
        }
    }
    return nullptr;
}
} // namespace CODEGEN

#include "scope.h"

#include "expression.h"


namespace CODEGEN {
std::vector<IR::Vector<IR::Node> *> P4Scope::scope;
std::set<cstring> P4Scope::used_names;
std::map<cstring, const IR::Type *> P4Scope::name_2_type_param;
std::map<cstring, const IR::Type *> P4Scope::name_2_type_param_in;
std::map<cstring, const IR::Type *> P4Scope::name_2_type_vars;
std::map<cstring, const IR::Type *> P4Scope::name_2_type_const;
std::map<cstring, const IR::Type *> P4Scope::name_2_type_stack;
std::map<cstring, std::map<int, std::set<cstring> > > P4Scope::lval_map;
std::map<cstring, std::map<int, std::set<cstring> > > P4Scope::lval_map_rw;


std::set<cstring> P4Scope::types_w_stack;
const IR::Type *P4Scope::ret_type = nullptr;
std::vector<IR::P4Control *> P4Scope::p4_ctrls;
std::map<cstring, IR::P4Control *> P4Scope::decl_ins_ctrls;
std::set<const IR::P4Table *> P4Scope::callable_tables;
const IR::Type_Struct *P4Scope::sys_hdr;
std::set<cstring> P4Scope::not_initialized_structs = {
    "ingress_intrinsic_metadata_t",
    "ingress_intrinsic_metadata_from_parser_t",
    "ingress_intrinsic_metadata_for_deparser_t",
    "egress_intrinsic_metadata_t",
    "egress_intrinsic_metadata_from_parser_t",
    "egress_intrinsic_metadata_for_deparser_t",
    "egress_intrinsic_metadata_for_output_port_t",
};
// refactor
int P4Scope::scope_indicator = SCOPE_PROGRAM;
std::map<cstring, const IR::Type_StructLike *> P4Scope::compound_type; // name for quick search

void P4Scope::add_to_scope(const IR::Node *n) {
    auto l_scope = P4Scope::scope.back();

    l_scope->push_back(n);

    //  if it is Type_StructLike, then add to compound_type
    if (auto ts = n->to<IR::Type_StructLike>()) {
        P4Scope::compound_type.emplace(ts->name.name, ts);
    }
}


void P4Scope::end_local_scope() {
    IR::Vector<IR::Node> *local_scope = scope.back();

    for (auto node : *local_scope) {
        if (auto decl = node->to<IR::Declaration>()) {
            name_2_type_param.erase(decl->name.name);
            name_2_type_param_in.erase(decl->name.name);
            name_2_type_vars.erase(decl->name.name);
            name_2_type_const.erase(decl->name.name);
            name_2_type_stack.erase(decl->name.name);
        }

        if (auto decl = node->to<IR::Declaration_Variable>()) {
            delete_lval(decl->type, decl->name.name);
        } else if (auto param = node->to<IR::Parameter>()) {
            delete_lval(param->type, param->name.name);
        }
    }

    delete local_scope;
    scope.pop_back();
    // clear the expressions
    expression::clear_data_structs();
    // clear the declaration instances
    decl_ins_ctrls.clear();
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
                           cstring                   sl_name) {
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
        type_key   = IR::Type_Bits::static_type_name();
        bit_bucket = tb->width_bits();
    } else if (auto tn = tp->to<IR::Type_Name>()) {
        auto tn_name = tn->path->name.name;
        if (tn_name == "packet_in") {
            return;
        }
        if (compound_type.count(tn_name) != 0) {
            auto tn_type = compound_type[tn_name];
            //width_bits should work here, do not know why not...
            type_key = tn_name;
            // does not work for some reason...
            // bit_bucket = P4Scope::compound_type[tn_name]->width_bits();
            bit_bucket = 1;
            delete_compound_lvals(tn_type, name);
        } else {
            printf("Type %s does not exist\n", tn_name);
            return;
        }
    } else{
        BUG("Type %s not yet supported", tp->node_type_name());
    }
    lval_map[type_key][bit_bucket].erase(name);

    if (lval_map[type_key][bit_bucket].size() == 0) {
        lval_map[type_key].erase(bit_bucket);
    }


    if (lval_map_rw.count(type_key) != 0) {
        if (lval_map_rw[type_key].count(bit_bucket) != 0) {
            lval_map_rw[type_key][bit_bucket].erase(name);
            if (lval_map_rw[type_key][bit_bucket].size() == 0) {
                lval_map_rw[type_key].erase(bit_bucket);
            }
        }
    }
}


void P4Scope::add_lval(const IR::Type *tp, cstring name, bool read_only) {
    cstring type_key;
    int bit_bucket;

    if (auto tb = tp->to<IR::Type_Bits>()) {
        type_key   = IR::Type_Bits::static_type_name();
        bit_bucket = tb->width_bits();
    } else if (auto tn = tp->to<IR::Type_Name>()) {
        auto tn_name = tn->path->name.name;
        if (tn_name == "packet_in") {
            return;
        }
        if (compound_type.count(tn_name) != 0) {
            auto tn_type = compound_type[tn_name];
            //width_bits should work here, do not know why not...
            type_key = tn_name;
            // does not work for some reason...
            // bit_bucket = P4Scope::compound_type[tn_name]->width_bits();
            bit_bucket = 1;
            add_compound_lvals(tn_type, name);
        } else {
            BUG("Type %s does not exist", tn_name);
        }
    } else{
        BUG("Type %s not yet supported", tp->node_type_name());
    }
    if (not read_only) {
        lval_map_rw[type_key][bit_bucket].insert(name);
    }
    lval_map[type_key][bit_bucket].insert(name);
}


std::set<cstring> P4Scope::get_candidate_lvals(const IR::Type *tp,
                                               bool           must_write) {
    cstring type_key;
    int bit_bucket;

    if (auto tb = tp->to<IR::Type_Bits>()) {
        type_key   = IR::Type_Bits::static_type_name();
        bit_bucket = tb->width_bits();
    } else if (auto tn = tp->to<IR::Type_Name>()) {
        auto tn_name = tn->path->name.name;
        if (P4Scope::compound_type.count(tn_name) != 0) {
            // bit_bucket = P4Scope::compound_type[tn_name]->width_bits();
            bit_bucket = 1;
        } else {
            BUG("Type_name refers to unknown type %s", tn_name);
        }
    } else{
        BUG("Type %s not yet supported", tp->node_type_name());
    }

    std::map<cstring, std::map<int, std::set<cstring> > > lookup_map;

    if (must_write) {
        lookup_map = lval_map_rw;
    } else{
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
    auto lval  = std::begin(candidates);
    // 'advance' the iterator n times
    std::advance(lval, idx);

    return *lval;
}


IR::Type_Bits *P4Scope::pick_declared_bit_type(bool must_write) {
    std::map<cstring, std::map<int, std::set<cstring> > > lookup_map;

    if (must_write) {
        lookup_map = lval_map_rw;
    } else{
        lookup_map = lval_map;
    }


    cstring bit_key = IR::Type_Bits::static_type_name();
    if (lookup_map.count(bit_key) == 0) {
        return nullptr;
    }
    auto key_types = lookup_map[bit_key];
    size_t idx     = rand() % key_types.size();
    int bit_width  = next(key_types.begin(), idx)->first;
    return new IR::Type_Bits(bit_width, false);
}


// Tao: filter is a mess here, sometimes it is filter, sometimes it is indicator
void P4Scope::get_all_type_names(cstring               filter,
                                 std::vector<cstring>& type_names) {
    for (auto i = scope.begin(); i < scope.end(); i++) {
        for (size_t j = 0; j < (*i)->size(); j++) {
            auto obj = (*i)->at(j);

            if (filter == HEADER) {
                if (obj->is<IR::Type_Declaration>() &&
                    !obj->is<IR::Type_Header>() &&
                    !obj->is<IR::Type_HeaderUnion>()) {
                    auto tmp_obj = obj->to<IR::Type_Declaration>();
                    type_names.push_back(tmp_obj->name.name);
                }
            } else if (filter == HEADER_UNION) {           // we only want header def
                if (obj->is<IR::Type_Header>()) {
                    auto tmp_obj = obj->to<IR::Type_Declaration>();
                    type_names.push_back(tmp_obj->name.name);
                }
            } else if ((filter == STRUCT) ||
                       (filter == STRUCT_HEADERS)) {
                if (!obj->is<IR::Type_Struct>() &&
                    !obj->is<IR::Type_Enum>() &&
                    !obj->is<IR::Type_SerEnum>() &&
                    obj->is<IR::Type_Declaration>()) {
                    if (obj->is<IR::Type_Typedef>()) {
                        auto tpdef_obj  = obj->to<IR::Type_Typedef>();
                        auto tpdef_name = tpdef_obj->type->to<IR::Type_Name>();
                        auto tpdef_type = get_type_by_name(
                            tpdef_name->path->name.name);
                        if (!(tpdef_type->is<IR::Type_Header>() ||
                              tpdef_type->is<IR::Type_HeaderUnion>())) {
                            continue;
                        }
                    }
                    auto tmp_obj = obj->to<IR::Type_Declaration>();
                    type_names.push_back(tmp_obj->name.name);
                }
            } else if (filter == STRUCT_LIKE) {
                if (obj->is<IR::Type_StructLike>()) {
                    auto tmp_obj = obj->to<IR::Type_StructLike>();
                    if ((tmp_obj->name.name != "standard_metadata_t") &&
                        (tmp_obj->name.name != "Meta") &&
                        (tmp_obj->name.name != "Headers")) {
                        type_names.push_back(tmp_obj->name.name);
                    }
                }
            } else if (filter == HEADER_ONLY) {
                if (obj->is<IR::Type_Header>() ||
                    obj->is<IR::Type_HeaderUnion>()) {
                    auto tmp_obj = obj->to<IR::Type_StructLike>();
                    type_names.push_back(tmp_obj->name.name);
                }
            } else {
                if (obj->is<IR::Type_Declaration>()) {
                    auto tmp_obj = obj->to<IR::Type_Declaration>();
                    type_names.push_back(tmp_obj->name.name);
                }
            }
        }
    }
}


std::vector<cstring> P4Scope::get_name_nodir_p4acts() {
    std::vector<cstring> ret;

    std::vector<const IR::P4Action *> nodir_p4acts;

    nodir_p4acts = get_p4actions_nodir();

    for (auto i = nodir_p4acts.begin(); i < nodir_p4acts.end(); i++) {
        ret.push_back((*i)->name.name);
    }

    return ret;
}


std::vector<const IR::P4Action *> P4Scope::get_p4actions_nodir() {
    std::vector<const IR::P4Action *> ret;
    auto p4actions_all = P4Scope::get_decls<IR::P4Action>();

    for (auto i = p4actions_all.begin(); i < p4actions_all.end(); i++) {
        const IR::P4Action *p4act = *i;
        auto act_params           = p4act->parameters->parameters;
        bool is_p4c_nodir         = true;

        for (size_t ind = 0; ind < act_params.size(); ind++) {
            auto param = act_params.at(ind);
            if (param->direction != IR::Direction::None) {
                is_p4c_nodir = false;
                break;
            }
        }

        if (is_p4c_nodir == true) {
            ret.push_back(p4act);
        }
    }

    return ret;
}


std::vector<const IR::Function *> P4Scope::get_func_decls() {
    return get_decls<IR::Function>();
}


std::vector<const IR::P4Table *> P4Scope::get_tab_decls() {
    return get_decls<IR::P4Table>();
}


std::vector<const IR::P4Action *> P4Scope::get_action_decls() {
    return get_decls<IR::P4Action>();
}


std::set<const IR::P4Table *> *P4Scope::get_callable_tables() {
    return &callable_tables;
}


template<typename T>
std::vector<const T *> P4Scope::get_decls() {
    std::vector<const T *> ret;

    for (auto i = scope.begin(); i < scope.end(); i++) {
        for (size_t j = 0; j < (*i)->size(); j++) {
            auto obj = (*i)->at(j);

            if (obj->is<T>()) {
                const T *tmp_obj = obj->to<T>();
                ret.push_back(tmp_obj);
            }
        }
    }
    return ret;
}


int P4Scope::get_num_type_header() {
    int ret = 0;

    for (auto i = scope.begin(); i < scope.end(); i++) {
        for (size_t j = 0; j < (*i)->size(); j++) {
            auto obj = (*i)->at(j);
            if (obj->is<IR::Type_Header>()) {
                ret = ret + 1;
            }
        }
    }
    return ret;
}


IR::Type *P4Scope::get_type_by_name(cstring name) {
    for (auto i = scope.begin(); i < scope.end(); i++) {
        for (size_t j = 0; j < (*i)->size(); j++) {
            auto obj = (*i)->at(j);
            if (obj->is<IR::Type_Declaration>()) {
                auto decl = obj->to<IR::Type_Declaration>();
                if (decl->name.name == name) {
                    return (IR::Type *)decl;
                }
            }
        }
    }
    return nullptr;
}


void P4Scope::print_scope() {
    // for (auto i = scope.begin(); i<scope.end(); i++) {
    //  for (size_t j=0; j< (*i)->size(); j++) {
    //      std::cout << (*i)->at(j) << std::endl;
    //  }
    // }
}
} // namespace CODEGE

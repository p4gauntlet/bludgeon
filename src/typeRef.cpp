#include "typeRef.h"
#include "baseType.h"
#include "headerStackType.h"

namespace CODEGEN {

IR::Type *typeRef::pick_rnd_type(typeref_probs type_probs) {
    const std::vector<int64_t> &type_probs_vector = {
        type_probs.p4_bit,       type_probs.p4_signed_bit,
        type_probs.p4_varbit,    type_probs.p4_int,
        type_probs.p4_error,     type_probs.p4_bool,
        type_probs.p4_string,    type_probs.p4_enum,
        type_probs.p4_header,    type_probs.p4_header_stack,
        type_probs.p4_struct,    type_probs.p4_header_union,
        type_probs.p4_tuple,     type_probs.p4_void,
        type_probs.p4_match_kind};
    const std::vector<int64_t> &basetype_probs = {
        type_probs.p4_bool,   type_probs.p4_error, type_probs.p4_int,
        type_probs.p4_string, type_probs.p4_bit,   type_probs.p4_signed_bit,
        type_probs.p4_varbit};

    if (type_probs_vector.size() != 15) {
        BUG("pick_rnd_type: Type probabilities must be exact");
    }
    IR::Type *tp = nullptr;
    size_t idx = randind(type_probs_vector);
    switch (idx) {
    case 0: {
        // bit<>
        tp = baseType::gen_bit_type(false);
        break;
    }
    case 1: {
        // int<>
        tp = baseType::gen_bit_type(true);
        break;
    }
    case 2: {
        // varbit<>, this is not supported right now
        break;
    }
    case 3: {
        // int, this is not supported right now
        tp = baseType::gen_int_type();
        break;
    }
    case 4: {
        // error, this is not supported right now
        break;
    }
    case 5: {
        // bool
        tp = baseType::gen_bool_type();
        break;
    }
    case 6: {
        // string, this is not supported right now
        break;
    }
    case 7: {
        // enum, this is not supported right now
        break;
    }
    case 8: {
        // header
        auto l_types = P4Scope::get_decls<IR::Type_Header>();
        if (l_types.size() == 0) {
            tp = baseType::pick_rnd_base_type(basetype_probs);
            break;
        }
        auto candidate_type = l_types.at(get_rnd_int(0, l_types.size() - 1));
        auto type_name = candidate_type->name.name;
        // check if struct is forbidden
        if (P4Scope::not_initialized_structs.count(type_name) == 0) {
            tp = new IR::Type_Name(candidate_type->name.name);
        } else {
            tp = baseType::pick_rnd_base_type(basetype_probs);
        }
        break;
    }
    case 9: {
        tp = headerStackType::gen();
        break;
    }
    case 10: {
        // struct
        auto l_types = P4Scope::get_decls<IR::Type_Struct>();
        if (l_types.size() == 0) {
            tp = baseType::pick_rnd_base_type(basetype_probs);
            break;
        }
        auto candidate_type = l_types.at(get_rnd_int(0, l_types.size() - 1));
        auto type_name = candidate_type->name.name;
        // check if struct is forbidden
        if (P4Scope::not_initialized_structs.count(type_name) == 0) {
            tp = new IR::Type_Name(candidate_type->name.name);
        } else {
            tp = baseType::pick_rnd_base_type(basetype_probs);
        }
        break;
    }
    case 11: {
        // header union, this is not supported right now
        break;
    }
    case 12: {
        // tuple, this is not supported right now
        break;
    }
    case 13: {
        // void
        tp = new IR::Type_Void();
        break;
    }
    case 14: {
        // match kind, this is not supported right now
        break;
    }
    }
    if (not tp) {
        BUG("pick_rnd_type: Chosen type is Null!");
    }

    return tp;
}
} // namespace CODEGEN

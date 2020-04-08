

#include "prefixedType.h"

namespace CODEGEN {

IR::Type *prefixedType::gen(cstring for_type) {
    const IR::Type_Declaration *candidate_type = nullptr;
    if (for_type == HEADER) {
        std::set<cstring> filter;
        filter.insert(IR::Type_Header::static_type_name());
        filter.insert(IR::Type_HeaderUnion::static_type_name());
        auto l_types = P4Scope::get_filtered_decls(filter);
        if (l_types.size() == 0) {
            return nullptr;
        }
        candidate_type = l_types.at(rand() % l_types.size());
    } else if (for_type == HEADER_UNION) { // we only want header def
        auto l_types = P4Scope::get_decls<IR::Type_Header>();
        if (l_types.size() == 0) {
            return nullptr;
        }
        candidate_type = l_types.at(rand() % l_types.size());
    } else if ((for_type == STRUCT) || (for_type == STRUCT_HEADERS)) {
        std::set<cstring> filter;
        filter.insert(IR::Type_Struct::static_type_name());
        filter.insert(IR::Type_Enum::static_type_name());
        filter.insert(IR::Type_SerEnum::static_type_name());
        auto l_types = P4Scope::get_filtered_decls(filter);
        if (l_types.size() == 0) {
            return nullptr;
        }
        candidate_type = l_types.at(rand() % l_types.size());
    } else if (for_type == STRUCT_LIKE) {
        auto l_types = P4Scope::get_decls<IR::Type_StructLike>();
        if (l_types.size() == 0) {
            return nullptr;
        }
        candidate_type = l_types.at(rand() % l_types.size());
    } else if (for_type == HEADER_ONLY) {
        std::vector<const IR::Type_Declaration *> l_types;
        auto l_types_h = P4Scope::get_decls<IR::Type_Header>();
        auto l_types_hu = P4Scope::get_decls<IR::Type_HeaderUnion>();
        l_types.insert(l_types.end(), l_types_h.begin(), l_types_h.end());
        l_types.insert(l_types.end(), l_types_hu.begin(), l_types_hu.end());
        if (l_types.size() == 0) {
            return nullptr;
        }

        candidate_type = l_types.at(rand() % l_types.size());
    } else {
        auto l_types = P4Scope::get_decls<IR::Type_Declaration>();
        if (l_types.size() == 0) {
            return nullptr;
        }
        candidate_type = l_types.at(rand() % l_types.size());
    }

    // we do not want it to have a stack type already
    if (P4Scope::check_type_name(candidate_type->name.name)) {
        return nullptr;
    }

    return new IR::Type_Name(candidate_type->name.name);
}

} // namespace CODEGEN

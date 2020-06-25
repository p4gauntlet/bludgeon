#include "structTypeDeclaration.h"

#include "annotations.h"
#include "common.h"
#include "scope.h"
#include "structFieldList.h"
#include "headerStackType.h"
#include "baseType.h"

namespace CODEGEN {

IR::StructField *structTypeDeclaration::pick_field() {
    cstring field_name = randstr(4);
    std::vector<int64_t> percent = {PCT.STRUCTTYPEDECLARATION_FIELD_BASE,
                                    PCT.STRUCTTYPEDECLARATION_FIELD_STRUCT,
                                    PCT.STRUCTTYPEDECLARATION_FIELD_STACK};
    std::vector<int64_t> type_probs = {
        PCT.STRUCTTYPEDECLARATION_BASETYPE_BOOL,
        PCT.STRUCTTYPEDECLARATION_BASETYPE_ERROR,
        PCT.STRUCTTYPEDECLARATION_BASETYPE_INT,
        PCT.STRUCTTYPEDECLARATION_BASETYPE_STRING,
        PCT.STRUCTTYPEDECLARATION_BASETYPE_BIT,
        PCT.STRUCTTYPEDECLARATION_BASETYPE_SIGNED_BIT,
        PCT.STRUCTTYPEDECLARATION_BASETYPE_VARBIT};
    IR::Type *tp = nullptr;
    bool fallback = false;
    switch (randind(percent)) {
    case 0: {
        fallback = true;
        break;
    }
    case 1: {
        // This is buggy right now, headers should be able to have structs...
        // TODO: Take a closer look
        auto l_types = P4Scope::get_decls<IR::Type_Struct>();
        if (l_types.size() == 0) {
            fallback = true;
            break;
        }
        auto candidate_type = l_types.at(get_rnd_int(0, l_types.size() - 1));
        tp = new IR::Type_Name(candidate_type->name.name);
        break;
    }
    case 2: {
        tp = headerStackType::gen();
    }
    }
    if (fallback) {
        tp = baseType::pick_rnd_base_type(type_probs);
    }

    if (auto struct_tp = tp->to<IR::Type_StructLike>()) {
        tp = new IR::Type_Name(struct_tp->name);
    }
    return new IR::StructField(field_name, tp);
}

IR::Type_Struct *structTypeDeclaration::gen() {
    cstring name = randstr(6);

    IR::IndexedVector<IR::StructField> fields;
    auto l_types = P4Scope::get_decls<IR::Type_Header>();
    if (l_types.size() == 0) {
        return nullptr;
    }
    size_t len = get_rnd_int(1, 5);

    for (size_t i = 0; i < len; i++) {
        auto *field = pick_field();
        if (field->type->to<IR::Type_Stack>()) {
            // Right now there is now way to initialize a header stack
            // So we have to add the entire structure to the banned expressions
            P4Scope::not_initialized_structs.insert(name);
        }
        fields.push_back(field);
    }

    auto ret = new IR::Type_Struct(name, fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

IR::Type_Struct *structTypeDeclaration::gen_Headers() {
    IR::IndexedVector<IR::StructField> fields;

    // Tao: hard code for ethernet_t eth_hdr;
    auto eth_sf = new IR::StructField(ETH_HDR, new IR::Type_Name(ETH_HEADER_T));
    fields.push_back(eth_sf);

    size_t len = get_rnd_int(1, 5);
    // we can only generate very specific types for headers
    // header, header stack, header union
    std::vector<int64_t> percent = {PCT.STRUCTTYPEDECLARATION_HEADERS_HEADER,
                                    PCT.STRUCTTYPEDECLARATION_HEADERS_STACK};
    for (size_t i = 0; i < len; i++) {
        cstring field_name = randstr(4);
        IR::Type *tp = nullptr;
        switch (randind(percent)) {
        case 0: {
            // TODO: We have to assume that this works
            auto l_types = P4Scope::get_decls<IR::Type_Header>();
            if (l_types.size() == 0) {
                BUG("structTypeDeclaration: No available header for Headers!");
            }
            auto candidate_type =
                l_types.at(get_rnd_int(0, l_types.size() - 1));
            tp = new IR::Type_Name(candidate_type->name.name);
            break;
        }
        case 1: {
            tp = headerStackType::gen();
            // Right now there is now way to initialize a header stack
            // So we have to add the entire structure to the banned expressions
            P4Scope::not_initialized_structs.insert("Headers");
        }
        }
        fields.push_back(new IR::StructField(field_name, tp));
    }
    auto ret = new IR::Type_Struct("Headers", fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

} // namespace CODEGEN

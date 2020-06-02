

#include "structTypeDeclaration.h"

namespace CODEGEN {

IR::StructField *structTypeDeclaration::pick_field() {
    cstring field_name = randstr(4);
    std::vector<int64_t> percent = {75, 20, 0};

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
        auto candidate_type = l_types.at(rand() % l_types.size());
        tp = new IR::Type_Name(candidate_type->name.name);
        break;
    }
    case 2: {
        tp = headerStackType::gen();
    }
    }
    if (fallback) {
        std::vector<int> b_types = {0, 1}; // only bit<> and bool for now
        tp = baseType::pick_rnd_base_type(b_types);
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
    size_t len = rand() % 5 + 1;

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

    size_t len = rand() % 5 + 1;
    // we can only generate very specific types for headers
    // header, header stack, header union
    std::vector<int64_t> percent = {90, 10};
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
            auto candidate_type = l_types.at(rand() % l_types.size());
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

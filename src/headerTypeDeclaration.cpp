

#include "headerTypeDeclaration.h"

namespace CODEGEN {

IR::Type *pick_field() {
    std::vector<int64_t> percent = {PCT.HEADERTYPEDECLARATION_FIELD_BASE,
                                    PCT.HEADERTYPEDECLARATION_FIELD_STRUCT};
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
    }
    if (fallback) {
        std::vector<int> b_types = {1}; // only bit<>
        tp = baseType::pick_rnd_base_type(b_types);
    }
    return tp;
}

IR::Type_Header *headerTypeDeclaration::gen() {
    cstring name = randstr(6);
    IR::IndexedVector<IR::StructField> fields;
    size_t len = get_rnd_int(1, 5);
    for (size_t i = 0; i < len; i++) {
        cstring field_name = randstr(4);
        IR::Type *field_tp = pick_field();

        if (auto struct_tp = field_tp->to<IR::Type_Struct>()) {
            field_tp = new IR::Type_Name(struct_tp->name);
        }
        IR::StructField *sf = new IR::StructField(field_name, field_tp);
        fields.push_back(sf);
    }
    auto ret = new IR::Type_Header(name, fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

IR::Type_Header *headerTypeDeclaration::gen_eth() {
    IR::IndexedVector<IR::StructField> fields;
    auto eth_dst =
        new IR::StructField("dst_addr", new IR::Type_Bits(48, false));
    auto eth_src =
        new IR::StructField("src_addr", new IR::Type_Bits(48, false));
    auto eth_type =
        new IR::StructField("eth_type", new IR::Type_Bits(16, false));

    fields.push_back(eth_dst);
    fields.push_back(eth_src);
    fields.push_back(eth_type);

    auto ret = new IR::Type_Header(IR::ID(ETH_HEADER_T), fields);
    P4Scope::add_to_scope(ret);

    return ret;
}

} // namespace CODEGEN



#include "structTypeDeclaration.h"

namespace CODEGEN {

IR::Type_Struct *structTypeDeclaration::gen() {
    cstring name = randstr(6);

    IR::IndexedVector<IR::StructField> fields;
    auto l_types = P4Scope::get_decls<IR::Type_Header>();
    if (l_types.size() == 0) {
        return nullptr;
    }
    size_t len = rand() % 5 + 1;
    for (size_t i = 0; i < len; i++) {
        cstring field_name = randstr(4);
        auto hdr_tp = l_types.at(rand() % l_types.size());
        auto tp_name = new IR::Type_Name(hdr_tp->name);
        IR::StructField *sf = new IR::StructField(field_name, tp_name);
        fields.push_back(sf);
    }

    auto ret = new IR::Type_Struct(name, fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

IR::Type_Struct *structTypeDeclaration::gen_Headers() {

    IR::IndexedVector<IR::StructField> fields;
    auto l_types = P4Scope::get_decls<IR::Type_Header>();
    if (l_types.size() == 0) {
        return nullptr;
    }
    size_t len = rand() % 5 + 1;
    for (size_t i = 0; i < len; i++) {
        cstring field_name = randstr(4);
        auto hdr_tp = l_types.at(rand() % l_types.size());
        auto tp_name = new IR::Type_Name(hdr_tp->name);
        IR::StructField *sf = new IR::StructField(field_name, tp_name);
        fields.push_back(sf);
    }

    // Tao: hard code for ethernet_t eth_hdr;
    auto eth_sf = new IR::StructField(ETH_HDR, new IR::Type_Name(ETH_HEADER_T));

    fields.insert(fields.begin(), eth_sf);

    auto ret = new IR::Type_Struct("Headers", fields);

    P4Scope::sys_hdr = ret;
    P4Scope::add_to_scope(ret);

    return ret;
}

} // namespace CODEGEN

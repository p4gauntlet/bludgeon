

#include "headerTypeDeclaration.h"

namespace CODEGEN {
IR::Type_Header *headerTypeDeclaration::gen() {
    cstring name = randstr(6);
    IR::IndexedVector<IR::StructField> fields =
        structFieldList::gen(HEADER, name, rand() % 5 + 1);
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

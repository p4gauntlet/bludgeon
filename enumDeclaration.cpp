

#include "enumDeclaration.h"

namespace CODEGEN {

static IR::Type_Enum *gen_enum(cstring name) {
    auto decl_ids = identifierList::gen(3);

    auto ret = new IR::Type_Enum(name, decl_ids);

    P4Scope::add_to_scope(ret);
    return ret;
}

static IR::Type_SerEnum *gen_ser_enum(cstring name) {
    auto members = specifiedIdentifierList::gen(3);
    IR::Type_Bits *tp = bit_literal::gen(false);

    auto ret = new IR::Type_SerEnum(name, tp, members);

    P4Scope::add_to_scope(ret);
    return ret;
}

IR::Type *enumDeclaration::gen(int type) {
    cstring name = randstr(4);
    if (type == 0) {
        return gen_enum(name);
    } else {
        return gen_ser_enum(name);
    }
}

} // namespace CODEGEN

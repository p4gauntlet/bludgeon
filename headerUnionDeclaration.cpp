

#include "headerUnionDeclaration.h"

namespace CODEGEN {
IR::Type_HeaderUnion *headerUnionDeclaration::gen() {
    cstring name = randstr(6);

    IR::IndexedVector<IR::StructField> fields =
        structFieldList::gen(HEADER_UNION, name, rand() % 5 + 1);
    auto ret = new IR::Type_HeaderUnion(name, fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

} // namespace CODEGEN

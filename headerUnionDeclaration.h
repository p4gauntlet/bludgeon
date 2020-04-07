#ifndef _HEADERUNIONDECLARATION_H_
#define _HEADERUNIONDECLARATION_H_

#include "ir/ir.h"

#include "scope.h"
#include "structFieldList.h"

#include "common.h"

namespace CODEGEN {

class headerUnionDeclaration {
  public:
    const char *types[0] = {};

    headerUnionDeclaration() {}

    static IR::Type_HeaderUnion *gen() {
        IR::ID *name;
        name = new IR::ID(CODEGEN::randstr(6));

        auto sfl = new structFieldList(HEADER_UNION, name->name);
        IR::IndexedVector<IR::StructField> fields = sfl->gen(rand() % 5 + 1);
        auto ret = new IR::Type_HeaderUnion(*name, fields);

        P4Scope::add_to_scope(ret);

        return ret;
    }
};

} // namespace CODEGEN

#endif

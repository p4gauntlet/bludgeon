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

    static IR::Type_HeaderUnion *gen();
};

} // namespace CODEGEN

#endif

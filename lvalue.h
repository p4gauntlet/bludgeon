#ifndef _LVALUE_H_
#define _LVALUE_H_

#include "ir/ir.h"

#include "codegen.h"
#include "scope.h"

namespace CODEGEN {

class lvalue {
  public:
    const char *types[1] = {"prefixedNonTypeName"};

    lvalue() {}
};

} // namespace CODEGEN

#endif

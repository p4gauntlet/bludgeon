#ifndef _FUNCTIONDECLARATION_H_
#define _FUNCTIONDECLARATION_H_

#include "blockStatement.h"
#include "expression.h"
#include "parameterList.h"
#include "scope.h"

#include "ir/ir.h"

namespace CODEGEN {
class functionDeclaration {
  public:
    const char *types[0] = {};

    functionDeclaration() { }

    ~functionDeclaration() { }

    static IR::Function *gen();
};
} // namespace CODEGEN

#endif

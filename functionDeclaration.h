#ifndef _FUNCTIONDECLARATION_H_
#define _FUNCTIONDECLARATION_H_

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

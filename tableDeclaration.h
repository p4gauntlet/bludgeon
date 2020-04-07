#ifndef _TABLEDECLARATION_H_
#define _TABLEDECLARATION_H_

#include "ir/ir.h"

#include "codegen.h"
#include "tablePropertyList.h"

namespace CODEGEN {
class tableDeclaration {
  public:
    const char *types[0] = {};

    tableDeclaration() {}

    ~tableDeclaration() {}

    static IR::P4Table *gen();
};
} // namespace CODEGEN

#endif

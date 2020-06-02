#ifndef _DECLARATION_H_
#define _DECLARATION_H_

#include "ir/ir.h"

namespace CODEGEN {
class declaration {
  public:
    const char *types[10] = {"constantDeclaration",  "externDeclaration",
                             "actionDeclaration",    "parserDeclaration",
                             "typeDeclaration",      "controlDeclaration",
                             "instantiation",        "errorDeclaration",
                             "matchKindDeclaration", "functionDeclaration"};

    declaration() {}
};
} // namespace CODEGEN

#endif

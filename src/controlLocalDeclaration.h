#ifndef _CONTROLLOCALDECLARATION_H_
#define _CONTROLLOCALDECLARATION_H_

#include "ir/ir.h"

namespace CODEGEN {

class controlLocalDeclaration {
  public:
    const char *types[5] = {"constantDeclaration", "actionDeclaration",
                            "tableDeclaration", "instantiation",
                            "variableDeclaration"};

    controlLocalDeclaration() {}
};

} // namespace CODEGEN

#endif
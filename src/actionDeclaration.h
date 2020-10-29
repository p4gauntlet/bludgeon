#ifndef _ACTIONDECLARATION_H_
#define _ACTIONDECLARATION_H_

#include "blockStatement.h"
#include "ir/ir.h"
#include "parameterList.h"

namespace CODEGEN {
class actionDeclaration {
 public:
    const char *types[0] = {};

    actionDeclaration() {}

    ~actionDeclaration() {}

    static IR::P4Action *gen();
};
} // namespace CODEGEN

#endif // ifndef _ACTIONDECLARATION_H_

#ifndef _ASSIGNMENTORMETHODCALLSTATEMENT_H_
#define _ASSIGNMENTORMETHODCALLSTATEMENT_H_

#include "ir/ir.h"

#include "expression.h"

namespace CODEGEN {
class assignmentOrMethodCallStatement {
  public:
    const char *types[0] = {};

    assignmentOrMethodCallStatement() {}

    static IR::AssignmentStatement *gen_assign();
    static IR::Statement *gen();
};
} // namespace CODEGEN

#endif

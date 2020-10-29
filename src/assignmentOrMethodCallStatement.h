#ifndef _ASSIGNMENTORMETHODCALLSTATEMENT_H_
#define _ASSIGNMENTORMETHODCALLSTATEMENT_H_

#include "ir/ir.h"

namespace CODEGEN {
class assignmentOrMethodCallStatement {
 public:
    const char *types[0] = {};

    assignmentOrMethodCallStatement() {}

    static IR::Statement *gen_assign();
    static IR::Statement *gen(bool is_in_func = false);
};
} // namespace CODEGEN

#endif

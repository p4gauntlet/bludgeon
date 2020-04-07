#ifndef _RETURNSTATEMENT_H_
#define _RETURNSTATEMENT_H_

#include "expression.h"
#include "scope.h"

#include "ir/ir.h"

namespace CODEGEN {
class returnStatement {
  public:
    const char *types[0] = {};

    returnStatement() {}

    ~returnStatement() {}

    static IR::ReturnStatement *gen_ret_stat(const IR::Type *tp = nullptr);
};
} // namespace CODEGEN

#endif

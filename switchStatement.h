#ifndef _SWITCHSTATEMENT_H_
#define _SWITCHSTATEMENT_H_

#include "common.h"
#include "expression.h"
#include "scope.h"

#include "blockStatement.h"
#include "ir/ir.h"

namespace CODEGEN {
class switchStatement {
  public:
    switchStatement() {}

    ~switchStatement() {
    }

    static IR::SwitchStatement *gen();
};
} // namespace CODEGEN

#endif

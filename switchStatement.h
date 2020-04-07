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
    IR::Expression *expr = nullptr;

    switchStatement() {}

    ~switchStatement() {
        if (expr != nullptr) {
            delete expr;
        }
    }

    IR::SwitchStatement *gen();
};
} // namespace CODEGEN

#endif

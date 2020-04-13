#include "argument.h"

#include "expression.h"
#include "scope.h"

namespace CODEGEN {

IR::Expression *argument::gen_input_arg(const IR::Parameter *param) {
    if (param->direction == IR::Direction::In) {
        return expression::gen_expr(param->type);
    } else {
        cstring lval = P4Scope::pick_lval(param->type, true);
        return new IR::PathExpression(lval);
    }
}

bool argument::check_input_arg(const IR::Parameter *param) {
    if (param->direction == IR::Direction::In) {
        return P4Scope::check_lval(param->type, false);
    } else {
        return P4Scope::check_lval(param->type, true);
    }
}

} // namespace CODEGEN

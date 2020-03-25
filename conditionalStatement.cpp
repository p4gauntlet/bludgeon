#include "conditionalStatement.h"
#include "statement.h"
#include "expression.h"


namespace CODEGEN {
IR::IfStatement *conditionalStatement::gen_if_stat(bool if_in_func) {
    IR::IfStatement *if_stat = nullptr;
    IR::Expression  *expr = nullptr;
    IR::Statement   *if_true = nullptr, *if_false = nullptr;

    expr     = expression::construct_cond_expr();
    if_true  = statement::gen_rnd(if_in_func);
    if_false = statement::gen_rnd(if_in_func);
    if ((expr != nullptr) && (if_true != nullptr) && (if_false != nullptr)) {
        if_stat = new IR::IfStatement(expr, if_true, if_false);
    }
    return if_stat;
}
} // namespace CODEGEN

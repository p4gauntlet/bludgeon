#include "conditionalStatement.h"
#include "expression.h"
#include "statement.h"

namespace CODEGEN {
IR::IfStatement *conditionalStatement::gen_if_stat(bool is_in_func) {
    IR::Expression *cond = nullptr;
    IR::Statement *if_true = nullptr, *if_false = nullptr;

    cond = expression::gen_expr(new IR::Type_Boolean());
    if (not cond) {
        BUG("cond in IfStatement should not be nullptr!");
    }
    if_true = statement::gen_rnd(is_in_func);
    if (not if_true) {
        // could not generate a statement
        // this happens when there is now way to generate an assignment
        if_true = new IR::EmptyStatement();
    }
    if_false = statement::gen_rnd(is_in_func);
    if (not if_false) {
        // could not generate a statement
        // this happens when there is now way to generate an assignment
        if_false = new IR::EmptyStatement();
    }
    return new IR::IfStatement(cond, if_true, if_false);
    ;
}
} // namespace CODEGEN

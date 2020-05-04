#include "returnStatement.h"

namespace CODEGEN {
IR::ReturnStatement *returnStatement::gen_ret_stat(const IR::Type *tp) {
    IR::Expression *expr = nullptr;

    // Type_Void is also empty
    if (tp && not tp->to<IR::Type_Void>()) {
        expr = expression::gen_expr(tp);
    }
    return new IR::ReturnStatement(expr);
}
} // namespace CODEGEN

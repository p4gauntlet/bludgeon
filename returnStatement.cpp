#include "returnStatement.h"

namespace CODEGEN {
IR::ReturnStatement *returnStatement::gen_ret_stat(const IR::Type *tp) {
    IR::Expression *expr = nullptr;

    if (tp) {
        expr = expression::gen_expr(tp);
    }
    return new IR::ReturnStatement(expr);
}
} // namespace CODEGEN

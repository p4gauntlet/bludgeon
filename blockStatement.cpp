#include "blockStatement.h"
#include "variableDeclaration.h"
#include "constantDeclaration.h"
#include "statementOrDeclaration.h"


namespace CODEGEN {

void blockStatement::gen_sth(bool if_in_func = false) {
    // randomize the total number of statements
    int max_statements = MIN_STAT + (rand() % (MAX_STAT - MIN_STAT + 1));

    // put tab_name .apply() after some initializations
    for (int num_stat = 0; num_stat <= max_statements; num_stat++) {
        IR::StatOrDecl *stmt = statementOrDeclaration::gen_rnd(
            if_in_func, if_in_ifs);
        if (stmt != nullptr) {
            stat_or_decls.push_back(stmt);
        }
    }
}


IR::BlockStatement *blockStatement::gen() {
    P4Scope::start_local_scope();

    gen_sth();

    P4Scope::end_local_scope();

    return new IR::BlockStatement(stat_or_decls);
}


IR::BlockStatement *blockStatement::gen_switch_blk() {
    P4Scope::start_local_scope();
    gen_sth();
    P4Scope::end_local_scope();
    return new IR::BlockStatement(stat_or_decls);
}


IR::BlockStatement *blockStatement::gen_func_blk(bool if_in_ifs = false) {
    P4Scope::start_local_scope();

    gen_sth(true);

    if (P4Scope::ret_type != nullptr) {
        const IR::Type_Bits *tp_bits = P4Scope::ret_type->to<IR::Type_Bits>();

        IR::Expression *expr      = nullptr;
        int            num_trials = 100;

        while (num_trials--) {
            expr = expression::construct_op_expr();
            if (expr != nullptr) {
                break;
            }
        }
        IR::ReturnStatement *ret_stat;

        if (expr == nullptr) {
            ret_stat = new IR::ReturnStatement(new IR::Constant(0));
        } else {
            ret_stat = new IR::ReturnStatement(new IR::Cast(tp_bits, expr));
        }
        stat_or_decls.push_back(ret_stat);
    }

    P4Scope::end_local_scope();
    return new IR::BlockStatement(stat_or_decls);
}
} // namespace CODEGEN

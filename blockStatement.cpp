#include "blockStatement.h"
#include "constantDeclaration.h"
#include "returnStatement.h"
#include "statementOrDeclaration.h"
#include "variableDeclaration.h"

namespace CODEGEN {

IR::IndexedVector<IR::StatOrDecl> gen_sth(bool if_in_func = false) {
    // randomize the total number of statements
    int max_statements = MIN_STAT + (rand() % (MAX_STAT - MIN_STAT + 1));
    IR::IndexedVector<IR::StatOrDecl> stat_or_decls;

    // put tab_name .apply() after some initializations
    for (int num_stat = 0; num_stat <= max_statements; num_stat++) {
        IR::StatOrDecl *stmt = statementOrDeclaration::gen_rnd(if_in_func);
        if (stmt != nullptr) {
            stat_or_decls.push_back(stmt);
        }
    }
    return stat_or_decls;
}

IR::BlockStatement *blockStatement::gen(bool if_in_func) {
    P4Scope::start_local_scope();

    auto stat_or_decls = gen_sth(if_in_func);

    if (if_in_func) {
        auto ret_stat = returnStatement::gen_ret_stat(P4Scope::ret_type);
        stat_or_decls.push_back(ret_stat);
    }
    P4Scope::end_local_scope();

    return new IR::BlockStatement(stat_or_decls);
}

} // namespace CODEGEN

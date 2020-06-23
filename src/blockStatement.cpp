#include "blockStatement.h"
#include "constantDeclaration.h"
#include "returnStatement.h"
#include "statementOrDeclaration.h"
#include "variableDeclaration.h"

namespace CODEGEN {

IR::IndexedVector<IR::StatOrDecl> gen_sth(bool is_in_func) {
    // randomize the total number of statements
    int max_statements =
        get_rnd_int(DECL.BLOCKSTATEMENT_MIN_STAT, DECL.BLOCKSTATEMENT_MAX_STAT);
    IR::IndexedVector<IR::StatOrDecl> stat_or_decls;

    // put tab_name .apply() after some initializations
    for (int num_stat = 0; num_stat <= max_statements; num_stat++) {
        IR::StatOrDecl *stmt = statementOrDeclaration::gen_rnd(is_in_func);
        if (stmt == nullptr) {
            BUG("Statement in BlockStatement should not be nullptr!");
        }
        stat_or_decls.push_back(stmt);
    }
    return stat_or_decls;
}

IR::BlockStatement *blockStatement::gen(bool is_in_func) {
    P4Scope::start_local_scope();

    auto stat_or_decls = gen_sth(is_in_func);

    if (is_in_func && not P4Scope::prop.ret_type->to<IR::Type_Void>()) {
        auto ret_stat = returnStatement::gen_ret_stat(P4Scope::prop.ret_type);
        stat_or_decls.push_back(ret_stat);
    }
    P4Scope::end_local_scope();

    return new IR::BlockStatement(stat_or_decls);
}

} // namespace CODEGEN

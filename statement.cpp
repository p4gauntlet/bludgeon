#include "statement.h"

#include "assignmentOrMethodCallStatement.h"
#include "blockStatement.h"
#include "conditionalStatement.h"
#include "exitStatement.h"
#include "returnStatement.h"
#include "switchStatement.h"

namespace CODEGEN {
IR::Statement *statement::gen_rnd(bool if_in_func = false) {
    std::vector<int> percent = {5, 75, 5, 5, 0, 5};
    IR::Statement *stmt = nullptr;

    switch (randind(percent)) {
    case 0: {
        switchStatement *switch_stat_gen = new switchStatement();
        stmt = switch_stat_gen->gen();
        break;
    }
    case 1: {
        stmt = assignmentOrMethodCallStatement::gen();
        break;
    }
    case 2: {
        stmt = conditionalStatement::gen_if_stat(if_in_func);
        break;
    }
    case 3: {
        stmt = returnStatement::gen_ret_stat(P4Scope::ret_type);
        break;
    }
    case 4: {
        if (!if_in_func) {
            stmt = exitStatement::gen();
        }
        break;
    }
    case 5: {
        auto blk_stat = new blockStatement();
        stmt = blk_stat->gen(if_in_func);
    }
    }
    return stmt;
}
} // namespace CODEGEN

#include "statementOrDeclaration.h"
#include "statement.h"
#include "variableDeclaration.h"

namespace CODEGEN {
IR::StatOrDecl *statementOrDeclaration::gen_rnd(bool if_in_func = false) {
    std::vector<int> percent = {10, 90};
    auto val = randind(percent);
    if (val == 0) {
        auto stmt = variableDeclaration::gen();
        if (not stmt) {
            BUG("Declaration in statementOrDeclaration should not be nullptr!");
        }
        return stmt;
    } else {
        auto stmt = statement::gen_rnd(if_in_func);
        if (not stmt) {
            BUG("Statement in statementOrDeclaration should not be nullptr!");
        }
        return stmt;
    }
}
} // namespace CODEGEN

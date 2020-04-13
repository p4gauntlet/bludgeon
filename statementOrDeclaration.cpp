#include "statementOrDeclaration.h"
#include "statement.h"
#include "variableDeclaration.h"

namespace CODEGEN {
IR::StatOrDecl *statementOrDeclaration::gen_rnd(bool is_in_func = false) {
    std::vector<int> percent = {10, 90};
    auto val = randind(percent);
    if (val == 0) {
        auto stmt = variableDeclaration::gen();
        if (not stmt) {
            BUG("Declaration in statementOrDeclaration should not be nullptr!");
        }
        return stmt;
    } else {
        auto stmt = statement::gen_rnd(is_in_func);
        if (not stmt) {
            // it can happen that no statement can be generated
            // for example in functions without writable values
            // so declare a variable instead
            return variableDeclaration::gen();
        }
        return stmt;
    }
}
} // namespace CODEGEN

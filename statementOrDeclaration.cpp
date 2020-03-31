#include "statementOrDeclaration.h"
#include "statement.h"
#include "variableDeclaration.h"

namespace CODEGEN {
IR::StatOrDecl *statementOrDeclaration::gen_rnd(bool if_in_func = false) {
    std::vector<int> percent = { 25, 75 };
    auto val = randind(percent);
    if (val == 0) {
        return  variableDeclaration::gen();
    } else {
        return statement::gen_rnd(if_in_func);
    }
}
} // namespace CODEGEN

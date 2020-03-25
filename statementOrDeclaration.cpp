#include "statementOrDeclaration.h"
#include "statement.h"
#include "variableDeclaration.h"

namespace CODEGEN {
IR::StatOrDecl *statementOrDeclaration::gen_rnd(bool if_in_func = false) {
    std::vector<int> percent = { 25, 75 };
    auto val = CODEGEN::randind(percent, 2);
    if (val == 0) {
        auto var_decl = new variableDeclaration();
        return var_decl->gen();
    } else {
        return statement::gen_rnd(if_in_func);
    }
}
} // namespace CODEGEN

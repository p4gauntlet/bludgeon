#include "statementOrDeclaration.h"
#include "statement.h"
#include "variableDeclaration.h"

namespace CODEGEN {
IR::StatOrDecl *statementOrDeclaration::gen_rnd(bool if_in_func = false,
                                                bool if_in_ifs  = false) {
    IR::StatOrDecl *stmt;
    std::vector<int> percent = { 75, 25 };

    switch (CODEGEN::randind(percent, 2)) {
        case 0: {
                stmt = statement::gen_rnd(if_in_func, if_in_ifs);
                break;
            }
        case 1: {
                auto var_decl = new variableDeclaration();
                stmt = var_decl->gen();
            }
    }
    return stmt;
}
} // namespace CODEGEN

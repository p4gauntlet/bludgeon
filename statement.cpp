#include "statement.h"


#include "assignmentOrMethodCallStatement.h"
#include "conditionalStatement.h"
#include "blockStatement.h"
#include "exitStatement.h"
#include "returnStatement.h"
#include "switchStatement.h"


namespace CODEGEN {
IR::Statement *statement::gen_rnd(bool if_in_func   = false,
                                  bool if_in_ifstat = false) {
    std::vector<int> percent = { 15, 15, 15, 15, 15, 15, 2, 2, 5 };
    IR::Statement *stmt      = nullptr;

    switch (CODEGEN::randind(percent, 9)) {
        case 0: {
                std::vector<cstring> tab_names = P4Scope::get_tab_names();
                if (tab_names.size() > 0) {
                    IR::Vector<IR::Argument> *args =
                        new IR::Vector<IR::Argument>();
                    auto tab_name = tab_names.at(
                        rand() % tab_names.size());

                    if (P4Scope::called_tables.find(tab_name) ==
                        P4Scope::called_tables.end()) {
                        auto mem =
                            new IR::Member(new IR::PathExpression(tab_name),
                                           IR::ID("apply"));
                        auto mce = new IR::MethodCallExpression(mem, args);
                        stmt = new IR::MethodCallStatement(mce);
                        P4Scope::called_tables.insert(tab_name);
                    }
                }
                break;
            }
        case 1: {
                switchStatement *switch_stat_gen = new switchStatement();
                stmt = switch_stat_gen->gen();
                break;
            }
        case 2: {
                stmt = assignmentOrMethodCallStatement::gen_assignstat();
                break;
            }
        case 3: {
                /*
                 * Tao: note here, may be a lot recursions,
                 *      so add some randomness here
                 */
                if (rand() % 4 == 0) {
                    stmt = conditionalStatement::gen_if_stat(if_in_func);
                }
                break;
            }
        case 4: {
                // Tao: generate action method call statement
                stmt =
                    assignmentOrMethodCallStatement::gen_act_methodcall_stat();
                break;
            }
        case 5: {
                // Tao: gen ctrl method call statement
                if (if_in_ifstat == false) {
                    stmt =
                        assignmentOrMethodCallStatement::gen_methodcall_stat();
                }
                break;
            }
        case 6: {
                stmt = returnStatement::gen_ret_stat();
                break;
            }
        case 7: {
                if ((if_in_func == false) && (if_in_ifstat == false)) {
                    stmt = exitStatement::gen();
                }
                break;
            }
        case 8: {
                std::vector<cstring> tab_names;
                auto blk_stat = new blockStatement(tab_names, false,
                                                   if_in_ifstat);
                if (if_in_func == true) {
                    stmt = blk_stat->gen_func_blk(if_in_ifstat);
                } else {
                    stmt = blk_stat->gen();
                }
            }
    }
    return stmt;
}
} // namespace CODEGEN

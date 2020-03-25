#include "switchStatement.h"

namespace CODEGEN {
IR::SwitchStatement *switchStatement::gen() {
    // get the expression
    auto tbl_set = P4Scope::get_callable_tables();

    // return nullptr if there are no tables left
    if (tbl_set->size() == 0) {
        return nullptr;
    }
    auto idx      = rand() % tbl_set->size();
    auto tbl_iter = std::begin(*tbl_set);

    std::advance(tbl_iter, idx);
    const IR::P4Table *tbl = *tbl_iter;
    auto expr =
        new IR::Member(new IR::MethodCallExpression(
                           new IR::Member(new IR::PathExpression(tbl->name),
                                          "apply")), "action_run");
    tbl_set->erase(tbl_iter);

    // get the switch cases
    IR::Vector<IR::SwitchCase> switch_cases;
    for (auto tab_property: tbl->properties->properties) {
        if (tab_property->name.name ==
            IR::TableProperties::actionsPropertyName) {
            auto property = tab_property->value->to<IR::ActionList>();
            for (auto action: property->actionList) {
                cstring act_name = action->getName();
                blockStatement blk_stat_gen = blockStatement();
                auto blk_stat = blk_stat_gen.gen();
                IR::SwitchCase *switch_case = new IR::SwitchCase(
                    new IR::PathExpression(act_name), blk_stat);
                switch_cases.push_back(switch_case);
            }
        }
    }

    return new IR::SwitchStatement(expr, switch_cases);
}
} // namespace CODEGEN




#include "switchStatement.h"

namespace CODEGEN {

IR::SwitchStatement *switchStatement::gen() {
    IR::SwitchStatement *ret = nullptr;
    auto tabs = P4Scope::get_tab_decls();
    if (tabs.size() == 0) {
        return ret;
    }

    auto tab = tabs.at(rand()%tabs.size());
    auto tab_name = tab->name.name;
    if (P4Scope::called_tables.find(tab_name) !=
            P4Scope::called_tables.end()) {
        return ret;
    }

    P4Scope::called_tables.insert(tab_name);
    // get the expression
    expr = new IR::Member(new IR::MethodCallExpression(new IR::Member(
                    new IR::PathExpression(new IR::Path(IR::ID(tab_name))), IR::ID("apply"))), IR::ID("action_run"));
    // get the switch cases
    IR::Vector<IR::SwitchCase> switch_cases;
    std::vector<cstring> empty_tab_names;
    for (size_t i=0; i<tab->properties->properties.size(); i++) {
        auto tab_property = tab->properties->properties.at(i);
        if (tab_property->name.name == IR::TableProperties::actionsPropertyName) {
            auto property = tab_property->value->to<IR::ActionList>();
            auto act_list = property->actionList;
            for (size_t j=0; j<act_list.size(); j++) {
                auto acl_ele = act_list.at(j);
                cstring act_name = acl_ele->expression->to<IR::MethodCallExpression>()->method->to<IR::PathExpression>()->path->name.name;
                blockStatement blk_stat_gen = blockStatement(empty_tab_names, false);
                auto blk_stat = blk_stat_gen.gen_switch_blk();
                IR::SwitchCase *switch_case = new IR::SwitchCase(new IR::PathExpression(new IR::Path(IR::ID(act_name))), blk_stat);
                switch_cases.push_back(switch_case);
            }
        }
    }

    return new IR::SwitchStatement(expr, switch_cases);
}

} // namespace CODEGEN


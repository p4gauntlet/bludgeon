#include "actionList.h"
#include "expression_2.h"

namespace CODEGEN {
IR::MethodCallExpression *gen_actioncall_expression(
    cstring           method_name,
    IR::ParameterList params) {
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
    IR::IndexedVector<IR::StatOrDecl> decls;

    bool can_call = true;

    for (auto par: params) {
        if (not expression2::check_input_arg(par)) {
            can_call = false;
        } else {
            IR::Argument *arg;
            arg = new IR::Argument(expression2::gen_input_arg(par));
            args->push_back(arg);
        }
    }
    if (can_call) {
        auto path_expr = new IR::PathExpression(method_name);
        return new IR::MethodCallExpression(path_expr, args);
    } else {
        return nullptr;
    }
}


IR::ActionList *actionList::gen(size_t len) {
    IR::IndexedVector<IR::ActionListElement> act_list;
    auto p4_actions = P4Scope::get_action_decls();
    std::set<cstring> act_names;

    if (p4_actions.size() == 0) {
        return new IR::ActionList(act_list);
    }
    for (size_t i = 0; i < len; i++) {
        size_t idx       = rand() % p4_actions.size();
        auto p4_act      = p4_actions[idx];
        cstring act_name = p4_act->name.name;

        if (act_names.find(act_name) != act_names.end()) {
            continue;
        }
        act_names.insert(act_name);

        auto params = p4_act->parameters;
        IR::MethodCallExpression *mce = gen_actioncall_expression(act_name,
                                                                  *params);
        if (mce) {
            IR::ActionListElement *actlist_ele = new IR::ActionListElement(
                mce);
            act_list.push_back(actlist_ele);
        }
    }
    return new IR::ActionList(act_list);
}
} // namespace CODEGEN

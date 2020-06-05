#include "actionList.h"

#include "argument.h"
#include "expression.h"
#include "scope.h"

namespace CODEGEN {
IR::MethodCallExpression *gen_actioncall_expression(cstring method_name,
                                                    IR::ParameterList params) {
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
    IR::IndexedVector<IR::StatOrDecl> decls;

    bool can_call = true;

    for (auto par : params) {
        if (not argument::check_input_arg(par)) {
            can_call = false;
        } else if (par->direction == IR::Direction::None) {
            // do nothing; in tables directionless parameters are
            // set by the control plane
            continue;
        } else {
            IR::Argument *arg;
            if (par->direction == IR::Direction::In) {
                // the generated expression needs to be compile-time known
                P4Scope::req.compile_time_known = true;
                arg = new IR::Argument(expression::gen_expr(par->type));
            } else {
                cstring lval = P4Scope::pick_lval(par->type, true);
                arg = new IR::Argument(new IR::PathExpression(lval));
            }
            args->push_back(arg);
        }
    }
    if (can_call) {
        auto path_expr = new IR::PathExpression(method_name);
        return new IR::MethodCallExpression(path_expr, args);
    } else {
        delete args;
        return nullptr;
    }
} // namespace CODEGEN

IR::ActionList *actionList::gen(size_t len) {
    IR::IndexedVector<IR::ActionListElement> act_list;
    auto p4_actions = P4Scope::get_decls<IR::P4Action>();
    std::set<cstring> act_names;

    if (p4_actions.size() == 0) {
        return new IR::ActionList(act_list);
    }
    for (size_t i = 0; i < len; i++) {
        size_t idx = get_rnd_int(0, p4_actions.size() - 1);
        auto p4_act = p4_actions[idx];
        cstring act_name = p4_act->name.name;

        if (act_names.find(act_name) != act_names.end()) {
            continue;
        }
        act_names.insert(act_name);

        auto params = p4_act->parameters;
        IR::MethodCallExpression *mce =
            gen_actioncall_expression(act_name, *params);
        if (mce) {
            IR::ActionListElement *actlist_ele = new IR::ActionListElement(mce);
            act_list.push_back(actlist_ele);
        }
    }
    return new IR::ActionList(act_list);
}
} // namespace CODEGEN

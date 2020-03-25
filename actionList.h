#ifndef _ACTIONLIST_H_
#define _ACTIONLIST_H_

#include "ir/ir.h"

#include "scope.h"
#include "expression.h"

namespace CODEGEN {
class actionList {
public:
    const char *types[0] = {
    };

    actionList() {
    }

    ~actionList() {
    }

    std::set<cstring> act_names;

    IR::ActionList *gen(size_t len) {
        IR::IndexedVector<IR::ActionListElement> act_list;
        std::map<cstring,
                 std::vector<const IR::Type *> > p4act_def =
            P4Scope::get_action_def();

        for (size_t i = 0; i < len; i++) {
            size_t ind = rand() % p4act_def.size();
            size_t cnt = 0;
            cstring act_name;
            for (auto& kv : p4act_def) {
                if (cnt == ind) {
                    act_name = kv.first;
                    break;
                }
                cnt++;
            }

            if (act_names.find(act_name) != act_names.end()) {
                continue;
            }
            act_names.insert(act_name);

            IR::ID *name = new IR::ID(act_name);
            IR::Expression *path_expr =
                new IR::PathExpression(new IR::Path(*name));
            auto params = p4act_def[act_name];
            IR::Vector<IR::Argument> *args =
                expression::construct_params(params);
            if (args->size() != params.size()) {
                act_names.erase(act_names.find(act_name));
                continue;
            }
            IR::MethodCallExpression *mc_expr = new IR::MethodCallExpression(
                path_expr, args);
            IR::ActionListElement *actlist_ele = new IR::ActionListElement(
                mc_expr);

            act_list.push_back(actlist_ele);
        }
        return new IR::ActionList(act_list);
    }
};
} // namespace CODEGEN



#endif

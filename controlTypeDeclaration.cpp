#include "controlTypeDeclaration.h"

namespace CODEGEN {
IR::Type_Control *controlTypeDeclaration::gen_ing_ctrl_type(int tf_flag) {
    cstring ing_name = "ingress";
    IR::ParameterList *param_list;

    if (tf_flag == 0) {
        param_list = parameterList::gen_ing_params();
    } else if (tf_flag == 1) {
        param_list = parameterList::gen_tf_ing_params();
    } else {
        BUG("tf_flag should be 0 or 1");
    }

    // add to the scope
    for (auto param : param_list->parameters) {
        P4Scope::add_to_scope(param);
        // add to the name_2_type
        // only add values that are not read-only to the modifiable types
        if (param->direction == IR::Direction::In) {
            P4Scope::add_lval(param->type, param->name.name, true);
        } else {
            P4Scope::add_lval(param->type, param->name.name, false);
        }
    }

    return new IR::Type_Control(ing_name, param_list);
}
} // namespace CODEGEN

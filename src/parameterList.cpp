

#include "parameterList.h"

namespace CODEGEN {

IR::ParameterList *parameterList::gen() {
    IR::IndexedVector<IR::Parameter> params;
    size_t total_params = get_rnd_int(0, 3);
    size_t num_dir_params = total_params ? get_rnd_int(0, total_params - 1) : 0;
    size_t num_directionless_params = total_params - num_dir_params;
    for (size_t i = 0; i < num_dir_params; i++) {
        IR::Parameter *param = parameter::gen(false);
        if (param == nullptr) {
            BUG("param is null");
        }
        params.push_back(param);
        // add to the scope
        P4Scope::add_to_scope(param);
        // only add values that are not read-only to the modifiable types
        if (param->direction == IR::Direction::In) {
            P4Scope::add_lval(param->type, param->name.name, true);
        } else {
            P4Scope::add_lval(param->type, param->name.name, false);
        }
    }
    for (size_t i = 0; i < num_directionless_params; i++) {
        IR::Parameter *param = parameter::gen(true);

        if (param == nullptr) {
            BUG("param is null");
        }
        params.push_back(param);
        // add to the scope
        P4Scope::add_to_scope(param);
        P4Scope::add_lval(param->type, param->name.name, true);
    }

    return new IR::ParameterList(params);
}

} // namespace CODEGEN

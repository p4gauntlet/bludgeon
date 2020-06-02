

#include "parameterList.h"

namespace CODEGEN {

IR::ParameterList *parameterList::gen(bool if_none_dir) {
    IR::IndexedVector<IR::Parameter> params;
    size_t len = rand() % 4;
    std::set<cstring> params_name;
    for (size_t i = 0; i < len; i++) {
        IR::Parameter *param = parameter::gen(if_none_dir);

        if (param == nullptr) {
            BUG("param is null");
        }

        if (params_name.find(param->name.name) != params_name.end()) {
            continue;
        }

        params_name.insert(param->name.name);

        params.push_back(param);

        // add to the scope
        P4Scope::add_to_scope(param);

        // only add values that are not read-only to the modifiable types
        if (param->direction == IR::Direction::In) {
            P4Scope::add_lval(param->type, param->name.name, true);
        } else {
            P4Scope::add_lval(param->type, param->name.name, false);
        }

        // add params of all directions
    }

    return new IR::ParameterList(params);
}

} // namespace CODEGEN

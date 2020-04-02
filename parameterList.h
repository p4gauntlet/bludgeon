#ifndef _PARAMETERLIST_H_
#define _PARAMETERLIST_H_

#include "ir/ir.h"

#include "parameter.h"
#include "expression.h"
#include "scope.h"

namespace CODEGEN {
class parameterList {
public:

    const char *types[1] = {
        "nonEmptyParameterList"
    };

    bool if_none_dir = false;
    std::set<cstring>params_name;
    IR::IndexedVector<IR::Parameter>params;

    parameterList(bool if_none_dir) :
        if_none_dir(if_none_dir) {}

    void gen_params(size_t len) {
        for (size_t i = 0; i < len; i++) {
            auto parame          = new parameter(if_none_dir);
            IR::Parameter *param = parame->gen();

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
    }

    IR::ParameterList* gen() {
        gen_params(rand() % 4);

        return new IR::ParameterList(params);
    }

    static IR::ParameterList* gen_ing_params() {
        IR::IndexedVector<IR::Parameter> pars;

        pars.push_back(parameter::gen_ing_headers());
        pars.push_back(parameter::gen_ing_meta());
        pars.push_back(parameter::gen_ing_sm());

        return new IR::ParameterList(pars);
    }

    static IR::ParameterList* gen_tf_ing_params() {
        IR::IndexedVector<IR::Parameter> pars;

        pars.push_back(parameter::gen_ing_headers());
        pars.push_back(parameter::gen_ing_meta());
        pars.push_back(parameter::gen_param(IR::Direction::In, "ig_intr_md",
                                            "ingress_intrinsic_metadata_t"));
        pars.push_back(parameter::gen_param(IR::Direction::In, "ig_prsr_md",
                                            "ingress_intrinsic_metadata_from_parser_t"));
        pars.push_back(parameter::gen_param(IR::Direction::InOut, "ig_dprsr_md",
                                            "ingress_intrinsic_metadata_for_deparser_t"));
        pars.push_back(parameter::gen_param(IR::Direction::InOut, "ig_tm_md",
                                            "ingress_intrinsic_metadata_for_tm_t"));

        return new IR::ParameterList(pars);
    }

    static IR::ParameterList* gen_par_params() {
        IR::IndexedVector<IR::Parameter> pars;

        pars.push_back(parameter::gen_pkt_in());
        pars.push_back(parameter::gen_out_header());
        pars.push_back(parameter::gen_ing_meta());
        pars.push_back(parameter::gen_ing_sm());

        return new IR::ParameterList(pars);
    }
};
} // namespace CODEGEN


#endif // ifndef _PARAMETERLIST_H_

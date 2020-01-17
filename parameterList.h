#ifndef _PARAMETERLIST_H_
#define _PARAMETERLIST_H_

#include "ir/ir.h"

#include "parameter.h"
#include "scope.h"

namespace CODEGEN {


class parameterList {
public:
	const char* types[1] = {
		"nonEmptyParameterList"
	};

	bool if_none_dir = false;
	std::set<cstring> params_name;
	IR::IndexedVector< IR::Parameter > params;

    parameterList(bool if_none_dir) :
		if_none_dir(if_none_dir) {
	}

	void gen_params(size_t len) {

		for (size_t i=0; i<len; i++) {
			auto parame = new parameter(if_none_dir);
			IR::Parameter* param = parame->gen();

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
			if (param->direction != IR::Direction::In) {
				P4Scope::add_name_2_type_p(param->name.name, param->type);
			}
			// add params of all directions 
			P4Scope::add_name_2_type_p_in(param->name.name, param->type);
		}

	}

	IR::ParameterList* gen() {
		gen_params(rand()%4);

		return new IR::ParameterList(params);
	}

	static IR::ParameterList* gen_ing_params() {
		IR::IndexedVector< IR::Parameter > pars;

		pars.push_back(parameter::gen_ing_headers());
		pars.push_back(parameter::gen_ing_meta());
		pars.push_back(parameter::gen_ing_sm());

		return new IR::ParameterList(pars);
	}

};


} // namespace CODEGEN




#endif

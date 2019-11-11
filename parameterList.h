#ifndef _PARAMETERLIST_H_
#define _PARAMETERLIST_H_

#include "ir/ir.h"

#include "parameter.h"

namespace CODEGEN {


class parameterList {
public:
	const char* types[1] = {
		"nonEmptyParameterList"
	};

	std::set<cstring> params_name;
	IR::IndexedVector< IR::Parameter > params;

	parameterList() {
	}

	void gen_params(size_t len) {

		for (size_t i=0; i<len; i++) {
			auto parame = new parameter();
			IR::Parameter* param = parame->gen();

			if (param == nullptr) {
				BUG("param is null");
			}

			if (params_name.find(param->name.name) != params_name.end()) {
				continue;
			}

			params_name.insert(param->name.name);

			params.push_back(param);
		}

	}

	IR::ParameterList* gen() {
		gen_params(rand()%4);

		return new IR::ParameterList(params);
	}

};


} // namespace CODEGEN




#endif

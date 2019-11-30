#ifndef _CONTROLTYPEDECLARATION_H_
#define _CONTROLTYPEDECLARATION_H_

#include "ir/ir.h"

#include "codegen.h"
#include "parameterList.h"

#include "scope.h"

namespace CODEGEN {


class controlTypeDeclaration {
public:
	const char* types[0] = {
	};

	IR::ID* name;
	IR::ParameterList* param_list;


	controlTypeDeclaration() {
		name = new IR::ID(CODEGEN::randstr(4));
	}
	~controlTypeDeclaration() {
		delete name;
		delete param_list;
	}


	IR::Type_Control* gen() {
		auto paramL_gen = new parameterList(false);
		param_list = paramL_gen->gen();
        // auto params = param_list->parameters;

        // add to the scope
		/* Tao: we have done it at params
        for (size_t i=0; i<params.size(); i++) {
            auto param = params.at(i);
            P4Scope::add_to_scope((IR::Node *)param);
            // add to the name_2_type
			if (param->direction != IR::Direction::In) {
				// Tao: we can not modify the params of IN direction
				P4Scope::add_name_2_type_p(param->name.toString(), param->type);
			}
        }*/

		auto ret = new IR::Type_Control(*name, param_list);
		return ret;
	}

	static IR::Type_Control* gen_ing_ctrl_type() {
		IR::ID* ing_name = new IR::ID("ingress");
		auto param_list = parameterList::gen_ing_params();
		auto params = param_list->parameters;

        // add to the scope
        for (size_t i=0; i<params.size(); i++) {
            auto param = params.at(i);
            P4Scope::add_to_scope((IR::Node *)param);
            // add to the name_2_type
			if (param->direction != IR::Direction::In) {
				// Tao: we can not modify the params of IN direction
				P4Scope::add_name_2_type_p(param->name.toString(), param->type);
			}
        }

		return new IR::Type_Control(*ing_name, param_list);
	}

};


} // namespace CODEGEN




#endif

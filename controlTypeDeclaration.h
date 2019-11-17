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


	IR::Type_Control* gen() {
		auto paramL_gen = new parameterList(false);
		param_list = paramL_gen->gen();
        auto params = param_list->parameters;

        // add to the scope
        for (size_t i=0; i<params.size(); i++) {
            auto param = params.at(i);
            P4Scope::add_to_scope((IR::Node *)param);
            // add to the name_2_type
            P4Scope::add_name_2_type_p(param->name.name, param->type);
        }

		auto ret = new IR::Type_Control(*name, param_list);
		return ret;
	}

};


} // namespace CODEGEN




#endif

#ifndef _CONTROLTYPEDECLARATION_H_
#define _CONTROLTYPEDECLARATION_H_

#include "ir/ir.h"

#include "codegen.h"
#include "parameterList.h"


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
		auto params = new parameterList(false);
		param_list = params->gen();

		auto ret = new IR::Type_Control(*name, param_list);
		return ret;
	}

};


} // namespace CODEGEN




#endif

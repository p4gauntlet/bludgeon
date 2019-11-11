#ifndef _VARIABLEDECLARATION_H_
#define _VARIABLEDECLARATION_H_

#include "ir/ir.h"


#include "common.h"
#include "codegen.h"


#include "typeRef.h"

namespace CODEGEN {


class variableDeclaration {
public:
	const char* types[0] = {
	};

	IR::ID* name;
	IR::Type* type;
	IR::Expression* expr;


	variableDeclaration() {
		name = new IR::ID(CODEGEN::randstr(6));
	}


	// Tao: maybe some customizations
	IR::Declaration_Variable* gen() {
		std::vector<int> types = {};
		typeRef* type_ref = new typeRef(true, types, "");
		type = type_ref->gen();


		auto ret = new IR::Declaration_Variable(*name, type);


		return ret;
	}
};


} // namespace CODEGEN




#endif

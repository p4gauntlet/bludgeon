#ifndef _VARIABLEDECLARATION_H_
#define _VARIABLEDECLARATION_H_

#include "ir/ir.h"


#include "common.h"
#include "codegen.h"
#include "scope.h"


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
		typeRef* type_ref = new typeRef(true, types, STRUCT);
		type = type_ref->gen();


		auto ret = new IR::Declaration_Variable(*name, type);
        P4Scope::add_to_scope((IR::Node *)ret);
        P4Scope::add_name_2_type_v(name->name, type);

		return ret;
	}
};


} // namespace CODEGEN




#endif

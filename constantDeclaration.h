#ifndef _CONSTANTDECLARATION_H_
#define _CONSTANTDECLARATION_H_

#include "ir/ir.h"

#include "expression.h"
#include "typeRef.h"

namespace CODEGEN {

class constantDeclaration {
public:
	const char* types[0] = {
	};

	IR::ID *name;
	IR::Expression* expr;
	IR::Type* tp;

	constantDeclaration() {
		name = new IR::ID(CODEGEN::randstr(4));
	}


	IR::Declaration_Constant* gen() {
	}
};


} // namespace CODEGEN




#endif

#ifndef _CONSTANTDECLARATION_H_
#define _CONSTANTDECLARATION_H_

#include "ir/ir.h"

#include "codegen.h"
#include "expression.h"
#include "baseType.h"

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
		std::vector<int> type;
		auto base_type = new baseType(true, type);
		tp = base_type->gen();

		if (tp->is<IR::Type_Boolean>()) {
			auto ex = new expression();
			expr = ex->gen_literal(0);
		} else {
			auto ex = new expression();
			expr = ex->gen_literal(1);
		}

		return new IR::Declaration_Constant(*name, tp, expr);
	}
};


} // namespace CODEGEN




#endif

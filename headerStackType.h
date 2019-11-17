#ifndef _HEADERSTACKTYPE_H_
#define _HEADERSTACKTYPE_H_

#include "ir/ir.h"


#include "typeName.h"
#include "expression.h"

#include "scope.h"

namespace CODEGEN {


class headerStackType {
public:
	const char* types[0] = {
	};

	cstring for_type;


	typeName* type_name;
	expression* expr;


	headerStackType(cstring for_type) : for_type(for_type) {
		type_name = new typeName(for_type);
		expr = new expression();
	}

	IR::Type* gen() {
		IR::Type* tp = type_name->gen();
		IR::Expression* e = expr->gen_literal(1);
		if (tp == nullptr || e == nullptr) {
			return nullptr;
		}
		IR::Type* ret =  new IR::Type_Stack(tp, e);

		return ret;
	}

};


} // namespace CODEGEN




#endif

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
	~headerStackType() {
		delete type_name;
		delete expr;
	}

	IR::Type* gen() {
		IR::Type* tp = type_name->gen();
		IR::Expression* e = expr->gen_literal(1);
		if (tp == nullptr || e == nullptr) {
			return nullptr;
		}
		auto tp_name = tp->to<IR::Type_Name>();

		// Tao: STRUCT_LIKE is used to generate Declaration_Variable
		// should not be a struct stack
		if (for_type==STRUCT_LIKE) {
			IR::Type* typed_name = P4Scope::get_type_by_name(tp_name->path->name.name);
			if (typed_name->is<IR::Type_Struct>()) {
				return nullptr;
			}
		}

		IR::Type* ret =  new IR::Type_Stack(tp, e);
		return ret;
	}

};


} // namespace CODEGEN




#endif

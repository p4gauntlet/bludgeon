#ifndef _RETURNSTATEMENT_H_
#define _RETURNSTATEMENT_H_


#include "scope.h"
#include "expression.h"

#include "ir/ir.h"

namespace CODEGEN {


class returnStatement {
public:
	const char* types[0] = {
	};

	returnStatement() {
	}
	~returnStatement() {
	}


	static IR::ReturnStatement* gen_ret_stat() {
		IR::ReturnStatement* ret_stat = nullptr;
		IR::Expression* expr = nullptr;
		if (P4Scope::ret_type == nullptr) {
			ret_stat = new IR::ReturnStatement(expr);
		}
		else {
			int size = P4Scope::ret_type->to<IR::Type_Bits>()->size;
			ret_stat = new IR::ReturnStatement(new IR::Cast(new IR::Type_Bits(size, false), 
					expression::construct_op_expr()));
		}
		return ret_stat;
	}

};


} // namespace CODEGEN




#endif

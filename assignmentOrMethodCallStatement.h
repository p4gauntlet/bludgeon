#ifndef _ASSIGNMENTORMETHODCALLSTATEMENT_H_
#define _ASSIGNMENTORMETHODCALLSTATEMENT_H_

#include "ir/ir.h"


#include "expression.h"

namespace CODEGEN {


class assignmentOrMethodCallStatement {
public:
	const char* types[0] = {
	};

	assignmentOrMethodCallStatement() {
	}

	static IR::AssignmentStatement* gen_assignstat() {
		IR::AssignmentStatement *assignstat = nullptr;
		IR::Expression *left=nullptr, *right=nullptr;
		const IR::Type *l_tp, *r_tp;
		left = expression::get_bit_operand(&l_tp);
		right = expression::construct_op_expr();
		if (left!=nullptr && right!=nullptr) {
			r_tp = expression::mp_expr_2_type[right];
			int l_size = l_tp->to<IR::Type_Bits>()->size;
			int r_size = r_tp->to<IR::Type_Bits>()->size;
			if (l_size != r_size) {
				assignstat = new IR::AssignmentStatement(left, 
						new IR::Cast(new IR::Type_Bits(l_size, false), right));
			}
			else {
				assignstat = new IR::AssignmentStatement(left, right);
			}
		}

		return assignstat;
	}

	static IR::AssignmentStatement* gen_compound_ass() {
		IR::AssignmentStatement *assignstat = nullptr;
		IR::Expression* expr1, * expr2;
		const IR::Type* tp1;
		const IR::Type* tp2;
		if (expression::get_two_compound_operands(&expr1, &expr2, &tp1, &tp2) == true) {
			assignstat = new IR::AssignmentStatement(expr1, expr2);
		}

		return assignstat;
	}

	// TODO: methodcall statement is a big headache

};


} // namespace CODEGEN




#endif

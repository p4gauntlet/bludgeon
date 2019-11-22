#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "ir/ir.h"


#include "bool.h"
#include "bit.h"
#include "int.h"

#include "codegen.h"
#include "scope.h"

#define EXPR_TYPE_BITS "type_bits"
#define EXPR_TYPE_BOOL "type_boolean"

namespace CODEGEN {


class expression {
public:
	const char* types[0] = {
	};


	static std::vector<IR::Expression *> boolean_exprs;
	static std::map<IR::Expression *, const IR::Type*> mp_expr_2_type;
	static std::vector<IR::Expression *> bit_exprs;
	static std::set<IR::Expression *> forbidden_exprs;

	expression (){
    }

	IR::Expression* gen_literal(int tp_literal) {
		if (tp_literal != -1) {
			switch (tp_literal) {
				case 0: return bool_literal::gen_literal(); break;
				case 1: return bit_literal::gen_literal(); break;
				case 2: return new IR::StringLiteral(CODEGEN::randstr(6)); break;
			}
		}

		return nullptr;
	}

    // maybe we can operate on IR::Declaration
    // expression[ expression ] ArrayIndex
    // expression[ expression : expression ] Slice
    // { expressionList } --> can be assigned to tuple, struct, header
    // { kvList }
    // ( expression )
    // !, ~, -, +

	static IR::Expression* get_operand(int pa_or_var, const IR::Type** tp, cstring &type);
	static IR::Expression* get_bit_operand(const IR::Type** tp);
	static IR::Expression* get_cond_expr();
	static IR::Expression* get_op_expr();
	static IR::Expression* construct_cond_expr();
	static IR::Expression* construct_op_expr();
	static IR::Expression* construct_l_val();


	static void clear_data_structs() {
		boolean_exprs.clear();
		mp_expr_2_type.clear();
		bit_exprs.clear();
		forbidden_exprs.clear();
	}
};


} // namespace CODEGEN




#endif

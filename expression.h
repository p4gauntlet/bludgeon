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
#define EXPR_TYPE_STRUCT "type_struct"
#define EXPR_TYPE_HEADER "type_header"
#define EXPR_TYPE_HUNION "type_hunion"

namespace CODEGEN {


class expression {
public:
	const char* types[0] = {
	};


	static std::vector<IR::Expression *> boolean_exprs;
	// hold types for bit_exprs
	static std::map<IR::Expression *, const IR::Type*> mp_expr_2_type;
	static std::vector<IR::Expression *> bit_exprs;
	static std::set<IR::Expression *> forbidden_exprs; // Tao: dont remember what it is for

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

	// basically, use get_operand to get a operand (bit or bool if not compound)
	static IR::Expression* get_operand(int pa_or_var, const IR::Type** tp, cstring &type, bool if_compound);
	// want a bit operand, not a bool
	static IR::Expression* get_bit_operand(const IR::Type** tp);
	// get an expression of cond or op
	static IR::Expression* get_cond_expr();
	static IR::Expression* get_op_expr();
	// construct more complex operations on cond and op
	static IR::Expression* construct_cond_expr();
	static IR::Expression* construct_op_expr();

	// TODO: some allowed operations on compound types
	// e.g. ==, != for cond, =,
	static bool get_two_compound_operands(IR::Expression** expr1, IR::Expression** expr2, const IR::Type** tp1, const IR::Type** tp2);
	static IR::Expression* construct_compound_cond_expr();
	static IR::Vector<IR::Argument> *construct_params(std::vector<const IR::Type*> &v_tp);

	static void clear_data_structs() {
		boolean_exprs.clear();
		mp_expr_2_type.clear();
		bit_exprs.clear();
		forbidden_exprs.clear();
	}
};


} // namespace CODEGEN




#endif

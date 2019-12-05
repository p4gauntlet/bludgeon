


#include "conditionalStatement.h"

namespace CODEGEN {

IR::IfStatement* conditionalStatement::gen_if_stat() {
	IR::IfStatement* if_stat = nullptr;
	IR::Expression* expr = nullptr;
	IR::Statement *if_true=nullptr, *if_false=nullptr;
	expr = expression::construct_cond_expr();
	if_true = statement::gen_stat(true);
	if_false = statement::gen_stat(true);
	if (expr!=nullptr && if_true!=nullptr && if_false!=nullptr) {
		if_stat = new IR::IfStatement(expr, if_true, if_false);
	}
	return if_stat;
}

} // namespace CODEGEN


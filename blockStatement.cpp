


#include "blockStatement.h"

namespace CODEGEN {

void blockStatement::gen_sth() {
	// auto const_decl = new constantDeclaration();
	// stat_or_decls.push_back(const_decl->gen());
	// Tao: generate tab_name .apply()
	for (auto &tab : tab_names) {
		IR::Vector<IR::Argument> * args = new IR::Vector<IR::Argument>();
		IR::ID apply("apply");
		IR::ID call_name(tab);
		IR::Member * mem = new IR::Member(new IR::PathExpression(new IR::Path(call_name)), apply);
		IR::MethodCallExpression * mce = new IR::MethodCallExpression(mem, args);
		stat_or_decls.push_back(new IR::MethodCallStatement(mce));
	}
	// initialize variables
	if (if_v_initialize == true) {
		auto ass_stats = expression::decl_v_initialize();
		for (auto i : ass_stats) {
			stat_or_decls.push_back(i);
		}
	}

	for (int cnt=0; cnt<5; cnt++) {
		auto ass = assignmentOrMethodCallStatement::gen_assignstat();
		if (ass != nullptr)
			stat_or_decls.push_back(ass);
		auto compound_ass = assignmentOrMethodCallStatement::gen_compound_ass();
		if (compound_ass != nullptr)
			stat_or_decls.push_back(compound_ass);
	}
	// Tao: note here, may be a lot recursions
	if (rand()%4 == 0) {
		auto if_stat = conditionalStatement::gen_if_stat();
		if (if_stat != nullptr) 
			stat_or_decls.push_back(if_stat);
	
	}
	// Tao: gen action method call statement
	for (int cnt=0; cnt<3; cnt++) {
		auto mcs = assignmentOrMethodCallStatement::gen_act_methodcall_stat();
		if (mcs != nullptr) {
			stat_or_decls.push_back(mcs);
		}
	}
	// Tao: gen ctrl method call statement
	for (int cnt=0; cnt<3; cnt++) {
		auto mcs = assignmentOrMethodCallStatement::gen_methodcall_stat();
		if (mcs != nullptr) {
			stat_or_decls.push_back(mcs);
		}
	}
}

IR::BlockStatement* blockStatement::gen() {

	gen_sth();

	return new IR::BlockStatement(stat_or_decls);
}

IR::BlockStatement* blockStatement::gen_func_blk() {
	// some variable declaration
	for (int i=0; i<15; i++) {
        auto var_decl = new variableDeclaration();
        stat_or_decls.push_back(var_decl->gen());
    }

	gen_sth();
	if (P4Scope::ret_type != nullptr) {
		const IR::Type_Bits *tp_bits = P4Scope::ret_type->to<IR::Type_Bits>();

		IR::Expression* expr = nullptr;
		int num_trials = 100;
		while (num_trials--) {
			expr = expression::construct_op_expr();
			if (expr != nullptr) {
				break;
			}
		}
		IR::ReturnStatement *ret_stat;
		if (expr == nullptr) {
			ret_stat = new IR::ReturnStatement(new IR::Constant(0));
		}
		else {
			ret_stat = new IR::ReturnStatement(new IR::Cast(tp_bits, expr));
		}
		stat_or_decls.push_back(ret_stat);
	}

	return new IR::BlockStatement(stat_or_decls);
}

} // namespace CODEGEN


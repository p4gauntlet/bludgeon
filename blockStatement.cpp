


#include "blockStatement.h"

namespace CODEGEN {

IR::BlockStatement* blockStatement::gen() {
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
	auto if_stat = conditionalStatement::gen_if_stat();
	if (if_stat != nullptr) 
		stat_or_decls.push_back(if_stat);

	return new IR::BlockStatement(stat_or_decls);
}

} // namespace CODEGEN





#include "statement.h"

namespace CODEGEN {

IR::Statement* statement::gen_stat() {
	IR::Statement* stat = nullptr;
	int num_trails = 100;
	while (num_trails--) {
		switch (rand()%5) {
			case 0: stat = assignmentOrMethodCallStatement::gen_assignstat(); break;
			// case 1: stat = assignmentOrMethodCallStatement::gen_compound_ass(); break;
			case 1: stat = conditionalStatement::gen_if_stat(); break;
			// case 2: stat = exitStatement::gen(); break;
			case 2: {
				std::vector<cstring> tab_names; // empty here
				auto blk_stat = new blockStatement(tab_names, false);
				stat = blk_stat->gen();
				break;
			}
			case 3: stat = returnStatement::gen_ret_stat(); break;
			case 4: stat = assignmentOrMethodCallStatement::gen_methodcall_stat(); break;
		}
		if (stat != nullptr) {
			break;
		}
	}
	return stat;
}

} // namespace CODEGEN


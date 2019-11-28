


#include "statement.h"

namespace CODEGEN {

IR::Statement* statement::gen_stat() {
	IR::Statement* stat = nullptr;
	int num_trails = 100;
	while (num_trails--) {
		switch (rand()%6) {
			case 0: stat = assignmentOrMethodCallStatement::gen_assignstat(); break;
			case 1: stat = assignmentOrMethodCallStatement::gen_compound_ass(); break;
			case 2: stat = conditionalStatement::gen_if_stat(); break;
			case 3: stat = exitStatement::gen(); break;
			case 4: {
				auto blk_stat = new blockStatement();
				stat = blk_stat->gen();
				break;
			}
			case 5: stat = returnStatement::gen_ret_stat(); break;
		}
		if (stat != nullptr) {
			break;
		}
	}
	return stat;
}

} // namespace CODEGEN


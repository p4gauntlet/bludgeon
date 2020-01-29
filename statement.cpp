


#include "statement.h"

namespace CODEGEN {

IR::Statement* statement::gen_stat(bool if_in_ifstat, bool if_in_func) {
	IR::Statement* stat = nullptr;
	int num_trails = 100;
	while (num_trails--) {
		switch (rand()%5) {
			case 0: stat = assignmentOrMethodCallStatement::gen_assignstat(); break;
			case 1: stat = assignmentOrMethodCallStatement::gen_compound_ass(); break;
			case 2: stat = conditionalStatement::gen_if_stat(if_in_func); break;
			case 3: {
				std::vector<cstring> tab_names; // empty here
				auto blk_stat = new blockStatement(tab_names, false, if_in_ifstat);
                if (if_in_func == true) {
                    stat = blk_stat->gen_func_blk();
                }
                else {
				    stat = blk_stat->gen();
                }
				break;
			}
			case 4: {
				if (if_in_ifstat == false)
					stat = assignmentOrMethodCallStatement::gen_methodcall_stat();
				break;
			}
			// case 5: stat = exitStatement::gen(); break;
			// case 6: stat = returnStatement::gen_ret_stat(); break;
		}
		if (stat != nullptr) {
			break;
		}
	}
	return stat;
}

} // namespace CODEGEN


#ifndef _BLOCKSTATEMENT_H_
#define _BLOCKSTATEMENT_H_

#include "ir/ir.h"

#include "constantDeclaration.h"
#include "assignmentOrMethodCallStatement.h"
#include "conditionalStatement.h"
#include "variableDeclaration.h"
#include "switchStatement.h"
#include "returnStatement.h"
#include "exitStatement.h"


#define NUM_STAT 10

namespace CODEGEN {


class blockStatement {
public:
	const char* types[0] = {
	};

	// Tao: please take care of the scope thing
	IR::IndexedVector< IR::StatOrDecl > stat_or_decls;
	std::vector<cstring> tab_names;
	bool if_v_initialize = false;
	bool if_in_ifs = false;

	blockStatement(std::vector<cstring> &tab_names, bool if_init, bool if_in_ifs=false) : 
			tab_names(tab_names),
			if_v_initialize(if_init),
			if_in_ifs(if_in_ifs) {
	}
	~blockStatement() {
	}

	// TODO: a lot of work needed to be done here

	IR::BlockStatement* gen();
    IR::BlockStatement* gen_switch_blk();
    void initialization();
	void gen_sth();
	IR::BlockStatement* gen_func_blk();
};


} // namespace CODEGEN




#endif

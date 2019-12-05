#ifndef _BLOCKSTATEMENT_H_
#define _BLOCKSTATEMENT_H_

#include "ir/ir.h"

#include "constantDeclaration.h"
#include "assignmentOrMethodCallStatement.h"
#include "conditionalStatement.h"
#include "variableDeclaration.h"

namespace CODEGEN {


class blockStatement {
public:
	const char* types[0] = {
	};

	// Tao: please take care of the scope thing
	IR::IndexedVector< IR::StatOrDecl > stat_or_decls;
	std::vector<cstring> tab_names;
	bool if_v_initialize = false;

	blockStatement(std::vector<cstring> &tab_names, bool if_init) : 
			tab_names(tab_names),
			if_v_initialize(if_init){
	}
	~blockStatement() {
	}

	// TODO: a lot of work needed to be done here

	IR::BlockStatement* gen();
	void gen_sth();
	IR::BlockStatement* gen_func_blk();
};


} // namespace CODEGEN




#endif

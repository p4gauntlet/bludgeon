#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include "ir/ir.h"

#include "assignmentOrMethodCallStatement.h"
#include "conditionalStatement.h"
#include "blockStatement.h"
#include "exitStatement.h"
#include "returnStatement.h"

namespace CODEGEN {


class statement {
public:
	const char* types[8] = {
		"assignmentOrMethodCallStatement",
		"directApplication",
		"conditionalStatement",
		"emptyStatement",
		"blockStatement",
		"exitStatement",
		"returnStatement",
		"switchStatement"
	};

	statement() {
	}
	~statement() {
	}

	static IR::Statement* gen_stat(bool if_in_ifstat, bool if_in_func);
};


} // namespace CODEGEN




#endif

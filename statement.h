#ifndef _STATEMENT_H_
#define _STATEMENT_H_

#include "ir/ir.h"

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

};


} // namespace CODEGEN




#endif
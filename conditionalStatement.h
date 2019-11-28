#ifndef _CONDITIONALSTATEMENT_H_
#define _CONDITIONALSTATEMENT_H_

#include "ir/ir.h"

#include "expression.h"
#include "statement.h"

namespace CODEGEN {


class conditionalStatement {
public:
	const char* types[0] = {
	};

	conditionalStatement() {
	}
	~conditionalStatement() {
	}

	static IR::IfStatement* gen_if_stat();
};


} // namespace CODEGEN




#endif

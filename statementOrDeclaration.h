#ifndef _STATEMENTORDECLARATION_H_
#define _STATEMENTORDECLARATION_H_

#include "ir/ir.h"

namespace CODEGEN {


class statementOrDeclaration {
public:
	const char* types[4] = {
		"variableDeclaration",
		"constantDeclaration",
		"statement",
		"instantiation"
	};

	statementOrDeclaration() {
	}

};


} // namespace CODEGEN




#endif
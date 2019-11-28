#ifndef _BLOCKSTATEMENT_H_
#define _BLOCKSTATEMENT_H_

#include "ir/ir.h"

#include "constantDeclaration.h"
#include "assignmentOrMethodCallStatement.h"
#include "conditionalStatement.h"

namespace CODEGEN {


class blockStatement {
public:
	const char* types[0] = {
	};

	// Tao: please take care of the scope thing
	IR::IndexedVector< IR::StatOrDecl > stat_or_decls;

	blockStatement() {
	}
	~blockStatement() {
	}

	// TODO: a lot of work needed to be done here

	IR::BlockStatement* gen();
};


} // namespace CODEGEN




#endif

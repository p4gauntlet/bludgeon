#ifndef _EXITSTATEMENT_H_
#define _EXITSTATEMENT_H_

#include "ir/ir.h"

namespace CODEGEN {


class exitStatement {
public:
	const char* types[0] = {
	};

	exitStatement() {
	}
	~exitStatement() {
	}

	static IR::ExitStatement* gen() {
		return new IR::ExitStatement();
	}

};


} // namespace CODEGEN




#endif

#ifndef _INT_H_
#define _INT_H_

#include "ir/ir.h"

namespace CODEGEN {
class int_literal {
public:
	int_literal() {
	}

	static IR::Type_InfInt* gen() {
		return new IR::Type_InfInt();
	}
	
};
}


#endif

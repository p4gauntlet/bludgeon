#ifndef _TYPEARG_H_
#define _TYPEARG_H_

#include "ir/ir.h"

namespace CODEGEN {


class typeArg {
public:
	const char* types[3] = {
		"DONTCARE",
		"typeRef",
		"nonTypeName"
	};

	typeArg() {
	}

};


} // namespace CODEGEN




#endif
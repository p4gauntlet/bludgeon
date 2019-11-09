#ifndef _TYPEORVOID_H_
#define _TYPEORVOID_H_

#include "ir/ir.h"

namespace CODEGEN {


class typeOrVoid {
public:
	const char* types[3] = {
		"typeRef",
		"VOID",
		"IDENTIFIER"
	};

	typeOrVoid() {
	}

};


} // namespace CODEGEN




#endif
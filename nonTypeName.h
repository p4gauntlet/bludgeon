#ifndef _NONTYPENAME_H_
#define _NONTYPENAME_H_

#include "ir/ir.h"

namespace CODEGEN {


class nonTypeName {
public:
	const char* types[7] = {
		"IDENTIFIER",
		"APPLY",
		"KEY",
		"ACTIONS",
		"STATE",
		"ENTRIES",
		"TYPE"
	};

	nonTypeName() {
	}

};


} // namespace CODEGEN




#endif
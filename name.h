#ifndef _NAME_H_
#define _NAME_H_

#include "ir/ir.h"

namespace CODEGEN {


class name {
public:
	const char* types[2] = {
		"nonTypeName",
		"TYPE_IDENTIFIER"
	};

	name() {
	}

};


} // namespace CODEGEN




#endif
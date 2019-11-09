#ifndef _NAMEDTYPE_H_
#define _NAMEDTYPE_H_

#include "ir/ir.h"

namespace CODEGEN {


class namedType {
public:
	const char* types[2] = {
		"typeName",
		"specializedType"
	};

	namedType() {
	}

};


} // namespace CODEGEN




#endif
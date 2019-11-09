#ifndef _TYPENAME_H_
#define _TYPENAME_H_

#include "ir/ir.h"


#include "prefixedType.h"

namespace CODEGEN {


class typeName {
public:
	const char* types[1] = {
		"prefixedType"
	};

	cstring for_type;
	typeName(cstring for_type) : for_type(for_type) {
	}

	IR::Type* gen() {
		auto prefixed_tp = new prefixedType(for_type);
		return prefixed_tp->gen();
	}
};


} // namespace CODEGEN




#endif

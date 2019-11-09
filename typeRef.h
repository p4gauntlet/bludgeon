#ifndef _TYPEREF_H_
#define _TYPEREF_H_

#include "ir/ir.h"

#include "baseType.h"
#include "typeName.h"
#include "headerStackType.h"

#include "common.h"


namespace CODEGEN {


class typeRef {
public:
	const char* types[5] = {
		"baseType",
		"typeName",
		"specializedType", // not 
		"headerStackType", 
		"tupleType" // not
	};

	cstring for_type;

	typeRef(cstring for_type) : for_type(for_type) {
	}

	IR::Type* gen() {
		int num_trials = 100;
		IR::Type* tp = nullptr;
		while (num_trials--) {
			switch (rand()%3) {
				case 0: {
							if (for_type == HEADER_UNION) {
								break;
							}
							tp = baseType::gen(); break;
						}
				case 1: {
							auto type_name = new typeName(for_type);
							tp = type_name->gen();
							break;
						}
				case 2: {
							if (for_type == HEADER_UNION) {
								break;
							}
							auto hdr_stack = new headerStackType(for_type);
							tp = hdr_stack->gen();
							break;
						}
			}

			if (tp != nullptr) {
				break;
			}
		}

		return tp;
	}
};


} // namespace CODEGEN




#endif

#ifndef _STRUCTFIELD_H_
#define _STRUCTFIELD_H_

#include "ir/ir.h"
#include "codegen.h"
#include "typeRef.h"




namespace CODEGEN {


class structField {
public:
	const char* types[0] = {
	};

	cstring for_type;

	structField(cstring for_type) : for_type(for_type) {
	}

	IR::StructField* gen() {
		IR::ID* name = new IR::ID(CODEGEN::randstr(4));

		std::vector<int> type;
		auto t_ref = new typeRef(true, type, for_type);
		IR::Type* tp = t_ref->gen();

		if (tp == nullptr) {
			return nullptr;
		}
		return new IR::StructField(*name, tp);
	}
};


} // namespace CODEGEN




#endif

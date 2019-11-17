#ifndef _HEADERTYPEDECLARATION_H_
#define _HEADERTYPEDECLARATION_H_

#include "ir/ir.h"

#include "scope.h"
#include "structFieldList.h"

#include "common.h"

namespace CODEGEN {


class headerTypeDeclaration {
public:
	const char* types[0] = {
	};

	headerTypeDeclaration() {
	}

	static IR::Type_Header* gen() {

		IR::ID *name;
		name = new IR::ID(CODEGEN::randstr(6));
		auto sfl = new structFieldList(HEADER, name->name);
		IR::IndexedVector< IR::StructField > fields = sfl->gen(rand()%10+1);
		auto ret = new IR::Type_Header(*name, fields);

		P4Scope::add_to_scope(ret);


		return ret;
	}
};


} // namespace CODEGEN




#endif

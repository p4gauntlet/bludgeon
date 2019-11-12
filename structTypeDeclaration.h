#ifndef _STRUCTTYPEDECLARATION_H_
#define _STRUCTTYPEDECLARATION_H_

#include "ir/ir.h"

#include "common.h"
#include "scope.h"

#include "annotations.h"
#include "structFieldList.h"



namespace CODEGEN {


class structTypeDeclaration {
public:
	const char* types[0] = {
	};


	structTypeDeclaration() {
	}

	static IR::Type_Struct* gen() {
		IR::ID *name;
		name = new IR::ID(CODEGEN::randstr(6));
		auto sfl = new structFieldList(HEADER);
		IR::IndexedVector< IR::StructField > fields = sfl->gen(rand()%10+1);

		auto ret = new IR::Type_Struct(*name, fields);

		P4Scope::add_to_scope(ret);

		return ret;
	}

};


} // namespace CODEGEN




#endif

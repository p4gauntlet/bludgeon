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
		auto sfl = new structFieldList(STRUCT, name->name);
		IR::IndexedVector< IR::StructField > fields = sfl->gen(rand()%5+1);

		auto ret = new IR::Type_Struct(*name, fields);

		P4Scope::add_to_scope(ret);

		return ret;
	}

    static IR::Type_Struct* gen_Headers() {
        IR::ID *name = new IR::ID("Headers");
		auto sfl = new structFieldList(STRUCT_HEADERS, name->name);
		IR::IndexedVector< IR::StructField > fields = sfl->gen(rand()%5+1);

		auto ret = new IR::Type_Struct(*name, fields);

        P4Scope::sys_hdr = ret;
		P4Scope::add_to_scope(ret);

		return ret;
    }

    static IR::Type_Struct* gen_Meta() {
        IR::ID *name = new IR::ID("Meta");
		auto sfl = new structFieldList(STRUCT, name->name);
		IR::IndexedVector< IR::StructField > fields = sfl->gen(rand()%5+1);

		auto ret = new IR::Type_Struct(*name, fields);

		P4Scope::add_to_scope(ret);

		return ret;
    }

	static IR::Type_Struct* gen_Sm() {
		IR::ID *name = new IR::ID("standard_metadata_t");
		auto fields = structFieldList::gen_sm();

		auto ret = new IR::Type_Struct(*name, fields);

		P4Scope::add_to_scope(ret);

		return ret;
	}

};


} // namespace CODEGEN




#endif

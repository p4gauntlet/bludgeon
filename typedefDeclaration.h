#ifndef _TYPEDEFDECLARATION_H_
#define _TYPEDEFDECLARATION_H_

#include "ir/ir.h"


#include "common.h"
#include "scope.h"
#include "codegen.h"
#include "typeRef.h"
#include "baseType.h"

#include "headerTypeDeclaration.h"
#include "structTypeDeclaration.h"
#include "headerUnionDeclaration.h"


namespace CODEGEN {


class typedefDeclaration {
public:
	const char* types[0] = {
	};


	IR::ID* name = nullptr; // name
	IR::Type* type = nullptr;


	typedefDeclaration() {
		name = new IR::ID(CODEGEN::randstr(5));
	}
	~typedefDeclaration() {
		if (name != nullptr) {
			delete name;
		}
		if (type != nullptr) {
			delete type;
		}
	}

	void gen_t() {
		while (1) {
			if (rand()%2 == 0) {
				std::vector<int> types;
				auto type_ref =  new typeRef(true, types, "");
				type = type_ref->gen();
			} else {
				switch (rand()%3) {
					case 0: type = headerTypeDeclaration::gen(); break;
					case 1: type = structTypeDeclaration::gen(); break;
					case 2: type = headerUnionDeclaration::gen(); break;
				}
			}

			if (type != nullptr) {
				break;
			}
		}
	}

	void gen_base_t() {
		std::vector<int> b_types;
		auto base_type = new baseType(true, b_types);
		type = base_type->gen();
	}

	IR::Type_Typedef* gen_typedef() {
		gen_t();
		auto ret = new IR::Type_Typedef(*name, type);
		P4Scope::add_to_scope(ret);
		return ret;
	}

	IR::Type_Newtype* gen_newtype() {
		gen_base_t();
		auto ret = new IR::Type_Newtype(*name, type);
		P4Scope::add_to_scope(ret);
		return ret;
	}

	IR::Type* gen() {
		if (rand()%2 == 0) {
			return gen_typedef();
		}
		else {
			return gen_newtype();
		}
	}

};


} // namespace CODEGEN




#endif

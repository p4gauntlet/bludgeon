#include "codegen.h"


#include "scope.h"
#include "headerTypeDeclaration.h"
#include "headerStackType.h"
#include "headerUnionDeclaration.h"
#include "structTypeDeclaration.h"
#include "enumDeclaration.h"
#include "actionDeclaration.h"

cstring CODEGEN::randstr(size_t len) {
	cstring ret;
	std::stringstream ss;
	while (1) {
		ss.str("");
		for (size_t i=0; i<len; i++) {
			ss << alphanum[rand() % (sizeof(alphanum) - 1)];
		}
		ret = ss.str();
		if (P4Scope::used_names.find(ret) == P4Scope::used_names.end()) {
			P4Scope::used_names.insert(ret);
			break;
		}
	}

	return ret;

}

namespace CODEGEN {


IR::Node* CGenerator::gen() {
	IR::Node* n = nullptr;
 	while (1) {
		switch (rand()%3) {
			case 0: {
						n = headerTypeDeclaration::gen();
						break;
					}
			case 1: {
						if (P4Scope::get_num_type_header() > 0) {
							n = headerUnionDeclaration::gen(); 
						}
						break;
					}
		}

		if (n != nullptr) {
			break;
		}
	}

	return n;
}

IR::Node* CGenerator::gen_act() {
	IR::Node* n = nullptr;
	while (1) {
		auto act_decl = new actionDeclaration();
		n = act_decl->gen();
		if (n != nullptr) {
			break;
		}
	}

	return n;
}

IR::Node* CGenerator::gen_struct() {
	IR::Node* n = nullptr;
	while (1) {
		n = structTypeDeclaration::gen();
		if (n != nullptr) {
			break;
		}
	}
	return n;
}

IR::Node* CGenerator::gen_t_enum() {
	IR::Node* n = nullptr;
	while (1) {
		auto t_enum = new enumDeclaration(rand()%2);
		n = t_enum->gen();
		if (n != nullptr) {
			break;
		}
	}
	return n;
}


} // namespace CODEGEN

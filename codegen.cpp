#include "codegen.h"


#include "scope.h"
#include "headerTypeDeclaration.h"
#include "headerStackType.h"
#include "headerUnionDeclaration.h"
#include "structTypeDeclaration.h"
#include "enumDeclaration.h"
#include "actionDeclaration.h"
#include "typedefDeclaration.h"
#include "controlDeclaration.h"
#include "tableDeclaration.h"
#include "actionList.h"
#include "functionDeclaration.h"


namespace CODEGEN {


IR::Node* CGenerator::gen() {
	IR::Node* n = nullptr;
 	while (1) {
		switch (rand()%2) {
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

IR::Node* CGenerator::gen_tpdef() {
	IR::Node* n = nullptr;
	while (1) {
		auto tpdef = new typedefDeclaration();
		n = tpdef->gen();
		if (n != nullptr) {
			break;
		}
	}
	return n;
}

IR::Node* CGenerator::gen_ctrldef() {
	IR::Node* n = nullptr;
	while (1) {
		auto ctrldef = new controlDeclaration();
		n = ctrldef->gen();
		if (n != nullptr) {
			break;
		}
	}
	return n;
}

IR::Node* CGenerator::gen_actlist() {
	auto names = P4Scope::get_name_nodir_p4acts();
	return nullptr;
}

IR::Node* CGenerator::gen_tab() {
    auto tab_gen = new tableDeclaration();
    return tab_gen->gen();
}

IR::Node* CGenerator::gen_func() {
	auto func_gen = new functionDeclaration();
	return func_gen->gen();
}


} // namespace CODEGEN

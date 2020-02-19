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
#include "p4parser.h"


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
		// auto t_enum = new enumDeclaration(rand()%2);
		// Tao: always generating SerEnum
		auto t_enum = new enumDeclaration(1);
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

IR::Node* CGenerator::gen_sys_parser() {
    auto p_gen = new p4Parser();
    return p_gen->gen_sys_p();
}

void CGenerator::emitBmv2Top(std::ostream *ostream) {
	*ostream << "#include <core.p4>\n";
	*ostream << "#include <v1model.p4>\n\n";
}

void CGenerator::emitBmv2Bottom(std::ostream *ostream) {
    *ostream << "parser p(packet_in b, out Headers h, inout Meta m, inout standard_metadata_t sm) {\n"
        "state start {"
        "transition accept;}}\n\n";

    *ostream << "control vrfy(inout Headers h, inout Meta m) { apply {} }\n\n";
    *ostream << "control update(inout Headers h, inout Meta m) { apply {} }\n\n";
    *ostream << "control egress(inout Headers h, inout Meta m, inout standard_metadata_t sm) { apply {} }\n\n";
    *ostream << "control deparser(packet_out b, in Headers h) { apply {} }\n\n";
    // *ostream << "control ingress(inout Headers h, inout Meta m, inout standard_metadata_t sm) { apply {} }\n\n";

    *ostream << "V1Switch(p(), vrfy(), ingress(), egress(), update(), deparser()) main;\n\n";
}

void CGenerator::gen_p4_code() {
	auto objects = new IR::Vector<IR::Node>();

    if (flag == 0) {
        CGenerator::emitBmv2Top(ostream);
    }
    else if (flag == 1) {
    }
    else {
        BUG("flag must be 0 or 1");
    }
    objects->push_back(CODEGEN::headerTypeDeclaration::gen_eth());
	objects->push_back(gen()); // generate hearder or header union
	objects->push_back(gen());
	objects->push_back(gen_struct()); // generate struct
	objects->push_back(gen_struct());
	objects->push_back(CODEGEN::structTypeDeclaration::gen_Headers()); // generate struct Headers
	objects->push_back(CODEGEN::structTypeDeclaration::gen_Meta()); // generate struct Meta
	CODEGEN::structTypeDeclaration::gen_Sm(); // generate struct standard_metadata_t
	// objects->push_back(>gen_ctrldef());
	objects->push_back(gen_func());
    // objects->push_back(cg->gen_sys_parser());
	objects->push_back(CODEGEN::controlDeclaration::gen_ing_ctrl());
	IR::P4Program *program = new IR::P4Program(*objects);

	// output to the file
    CODEGEN::SubToP4 top4(ostream, false);
	program->apply(top4);
    CGenerator::emitBmv2Bottom(ostream);
    ostream->flush();
}


} // namespace CODEGEN

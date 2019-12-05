#include <iostream>


#include "frontends/p4/toP4/toP4.h"


#include "ir/ir.h"
#include "lib/nullstream.h"

#include "codegen.h"
#include "scope.h"
#include "structTypeDeclaration.h"
#include "controlDeclaration.h"

void printUsage() {
	std::cout << "###How to use p4codegen###\n";
	std::cout << "./p4codegen [output_filename]\n";
}

void emitBottom(std::ostream* out) {
    *out << "parser p(packet_in b, out Headers h, inout Meta m, inout standard_metadata_t sm) {\n"
        "state start {"
        "transition accept;}}\n\n";

    *out << "control vrfy(inout Headers h, inout Meta m) { apply {} }\n\n";
    *out << "control update(inout Headers h, inout Meta m) { apply {} }\n\n";
    *out << "control egress(inout Headers h, inout Meta m, inout standard_metadata_t sm) { apply {} }\n\n";
    *out << "control deparser(packet_out b, in Headers h) { apply {} }\n\n";
    // *out << "control ingress(inout Headers h, inout Meta m, inout standard_metadata_t sm) { apply {} }\n\n";

    *out << "V1Switch(p(), vrfy(), ingress(), egress(), update(), deparser()) main;\n\n";
}

int main(int argc, char **argv) {

	srand(time(NULL));
	if (argc != 2) {
		printUsage();
		exit(-1);
	}

	// Tao: craft a new program

	CODEGEN::CGenerator *cg = new CODEGEN::CGenerator();



	auto objects = new IR::Vector<IR::Node>();
	objects->push_back(cg->gen());
	objects->push_back(cg->gen());
	// objects->push_back(cg->gen());
	// objects->push_back(cg->gen());
	objects->push_back(cg->gen_struct());
	objects->push_back(cg->gen_struct());
	objects->push_back(cg->gen_struct());
	objects->push_back(cg->gen_struct());
	objects->push_back(cg->gen_struct());
	objects->push_back(CODEGEN::structTypeDeclaration::gen_Headers());
	objects->push_back(CODEGEN::structTypeDeclaration::gen_Meta());
	CODEGEN::structTypeDeclaration::gen_Sm();
	// objects->push_back(cg->gen_ctrldef());
	// objects->push_back(cg->gen_ctrldef());
	// objects->push_back(cg->gen_func());
	// objects->push_back(cg->gen_func());
	// objects->push_back(cg->gen_func());
	objects->push_back(cg->gen_ctrldef());
	objects->push_back(CODEGEN::controlDeclaration::gen_ing_ctrl());
	IR::P4Program *program = new IR::P4Program(*objects);


	CODEGEN::P4Scope::print_scope();

	// end

	// output to the file
	auto ostream = openFile(argv[1], false);
	*ostream << "#include <core.p4>\n";
	*ostream << "#include <v1model.p4>\n\n";

    P4::ToP4 top4(ostream, false);

	program->apply(top4);

    emitBottom(ostream);

    ostream->flush();
	return 0;
}

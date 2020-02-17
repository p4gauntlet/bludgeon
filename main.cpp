#include <iostream>
#include <fstream>


#include "ir/ir.h"
#include "lib/nullstream.h"

#include "codegen.h"
#include "scope.h"
#include "sub_toP4.h"
#include "structTypeDeclaration.h"
#include "headerTypeDeclaration.h"
#include "controlDeclaration.h"
#include "p4parser.h"

void printUsage() {
	std::cout << "###How to use p4codegen###\n";
	std::cout << "./p4codegen [output_filename]\n";
}

unsigned int good_seed() {
    unsigned int random_seed, random_seed_a, random_seed_b;
    std::ifstream file ("/dev/urandom", std::ios::binary);
    if (file.is_open()) {
        char * memblock;
        int size = sizeof(int);
        memblock = new char [size];
        file.read (memblock, size);
        file.close();
        random_seed_a = *reinterpret_cast<int*>(memblock);
        delete[] memblock;
    } else {
        random_seed_a = 0;
    }
    random_seed_b = std::time(0);
    random_seed = random_seed_a xor random_seed_b;
    return random_seed;
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

	srand(good_seed());
	if (argc != 2) {
		printUsage();
		exit(-1);
	}

	// Tao: craft a new program
	CODEGEN::CGenerator *cg = new CODEGEN::CGenerator();

	auto objects = new IR::Vector<IR::Node>();
    objects->push_back(CODEGEN::headerTypeDeclaration::gen_eth());
	objects->push_back(cg->gen()); // generate hearder or header union
	objects->push_back(cg->gen());
	// objects->push_back(cg->gen_t_enum());
	// objects->push_back(cg->gen_t_enum());
	// objects->push_back(cg->gen_tpdef());
	// objects->push_back(cg->gen_tpdef());
	objects->push_back(cg->gen_struct()); // generate struct
	objects->push_back(cg->gen_struct());
	objects->push_back(CODEGEN::structTypeDeclaration::gen_Headers()); // generate struct Headers
	objects->push_back(CODEGEN::structTypeDeclaration::gen_Meta()); // generate struct Meta
	CODEGEN::structTypeDeclaration::gen_Sm(); // generate struct standard_metadata_t
	// objects->push_back(cg->gen_ctrldef());
	// objects->push_back(cg->gen_ctrldef());
	// objects->push_back(cg->gen_ctrldef());
	objects->push_back(cg->gen_func());
	// objects->push_back(cg->gen_func());
	// objects->push_back(cg->gen_func());
    // objects->push_back(cg->gen_sys_parser());
	objects->push_back(CODEGEN::controlDeclaration::gen_ing_ctrl());
	IR::P4Program *program = new IR::P4Program(*objects);

	CODEGEN::P4Scope::print_scope();
	// end

	// output to the file
	auto ostream = openFile(argv[1], false);
	*ostream << "#include <core.p4>\n";
	*ostream << "#include <v1model.p4>\n\n";

    CODEGEN::SubToP4 top4(ostream, false);

	program->apply(top4);

    emitBottom(ostream);

    ostream->flush();
	return 0;
}

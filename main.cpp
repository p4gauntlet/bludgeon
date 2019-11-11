#include <iostream>


#include "frontends/p4/toP4/toP4.h"


#include "ir/ir.h"
#include "lib/nullstream.h"

#include "codegen.h"
#include "scope.h"

void printUsage() {
	std::cout << "###How to use p4codegen###\n";
	std::cout << "./p4codegen [output_filename]\n";
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
	objects->push_back(cg->gen_struct());
	objects->push_back(cg->gen());
	objects->push_back(cg->gen_struct());
	objects->push_back(cg->gen());
	objects->push_back(cg->gen());
	objects->push_back(cg->gen_struct());
	objects->push_back(cg->gen());
	objects->push_back(cg->gen());
	objects->push_back(cg->gen_struct());
	objects->push_back(cg->gen_act());
	objects->push_back(cg->gen_act());
	objects->push_back(cg->gen_act());
	objects->push_back(cg->gen_t_enum());
	objects->push_back(cg->gen_t_enum());
	objects->push_back(cg->gen_t_enum());
	IR::P4Program *program = new IR::P4Program(*objects);


	CODEGEN::P4Scope::print_scope();

	// end

	// output to the file
	auto ostream = openFile(argv[1], false);
	*ostream << "#include <core.p4>\n";
	*ostream << "#include <v1model.p4>\n\n";

    P4::ToP4 top4(ostream, false);

	program->apply(top4);
	return 0;
}

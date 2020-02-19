#include <iostream>
#include <fstream>


#include "ir/ir.h"
#include "lib/nullstream.h"

#include "codegen.h"

void printUsage() {
	std::cout << "###How to use p4codegen###\n";
	std::cout << "./p4codegen [output_filename] [flag=0 for bmv2, flag=1 for tofino]\n";
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


int main(int argc, char **argv) {

	srand(good_seed());
	if (argc != 3) {
		printUsage();
		exit(-1);
	}

	// Tao: craft a new program
	CODEGEN::CGenerator *cg = new CODEGEN::CGenerator(argv[1], atoi(argv[2]));
    cg->gen_p4_code();

	return 0;
}

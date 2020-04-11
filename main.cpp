#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "frontends/p4/toP4/toP4.h"
#include "ir/ir.h"
#include "lib/nullstream.h"
#include "scope.h"
#include "tna.h"
#include "v1model.h"

void printUsage() {
    std::cout << "###How to use p4codegen###\n";
    std::cout << "./p4codegen [output_filename] [flag=0 for bmv2, flag=1 for "
                 "tofino]\n";
}

unsigned int good_seed() {
    unsigned int random_seed, random_seed_a, random_seed_b;
    std::ifstream file("/dev/urandom", std::ios::binary);

    if (file.is_open()) {
        char *memblock;
        int size = sizeof(int);
        memblock = new char[size];
        file.read(memblock, size);
        file.close();
        random_seed_a = *reinterpret_cast<int *>(memblock);
        delete[] memblock;
    } else {
        random_seed_a = 0;
    }
    random_seed_b = std::time(0);
    random_seed = random_seed_a xor random_seed_b;
    return random_seed;
}

namespace CODEGEN {
void gen_p4_code(cstring output_file, int flag) {
    std::ostream *ostream = openFile(output_file, false);
    P4Scope::start_local_scope();

    IR::P4Program *program = nullptr;
    if (flag == 0) {
        V1Model::generate_includes(ostream);
        program = V1Model::gen();
    } else if (flag == 1) {
        TNA::generate_includes(ostream);
        program = TNA::gen();
    } else {
        BUG("flag must be 0 or 1");
    }
    // output to the file
    P4::ToP4 top4(ostream, false);
    program->apply(top4);
    ostream->flush();
    P4Scope::end_local_scope();
}
} // namespace CODEGEN

int main(int argc, char **argv) {
    srand(good_seed());

    if (argc != 3) {
        printUsage();
        exit(-1);
    }
    cstring output_file = argv[1];
    int flag = atoi(argv[2]);

    CODEGEN::gen_p4_code(output_file, flag);

    return 0;
}

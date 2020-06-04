#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "frontends/p4/toP4/toP4.h"
#include "ir/ir.h"
#include "lib/gc.h"
#include "lib/crash.h"
#include "lib/nullstream.h"

#include "backends/tna.h"
#include "backends/top.h"
#include "backends/v1model.h"
#include "options.h"
#include "scope.h"
#include "version.h"


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
void gen_p4_code(cstring output_file, cstring target) {
    std::ostream *ostream = openFile(output_file, false);
    P4Scope::start_local_scope();

    IR::P4Program *program = nullptr;
    if (target == "v1model") {
        V1Model::generate_includes(ostream);
        program = V1Model::gen();
    } else if (target == "tna") {
        TNA::generate_includes(ostream);
        program = TNA::gen();
    } else if (target == "top") {
        Top::generate_includes(ostream);
        program = Top::gen();
    } else {
        ::error("Architecture must be v1model, tna, or top");
        return;
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
    setup_gc_logging();
    setup_signals();

    // add a dummy context
    AutoCompileContext autoBludgeonContext(new CODEGEN::P4CContext);
    auto options = CODEGEN::BludgeonOptions();
    // we only handle P4_16 right now
    options.compiler_version = CODEGEN_VERSION_STRING;

    options.process(argc, argv);
    if (::errorCount() > 0) {
        return 1;
    }
    // use a default name if no specific output name is provided
    if (options.output_file == nullptr) {
        options.output_file = "out.p4";
    }
    // use a default name if no specific output name is provided
    if (options.arch == nullptr) {
        ::error("--arch is required. Possible options: v1model, tna, or top");
        options.usage();
        return 1;
    }

    auto ostream = openFile(options.output_file, false);
    if (ostream == nullptr) {
        ::error("must have --output [file]");
        return 1;
    }

    CODEGEN::gen_p4_code(options.output_file, options.arch);

    return 0;
}

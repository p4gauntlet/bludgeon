#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "frontends/p4/toP4/toP4.h"
#include "ir/ir.h"
#include "lib/crash.h"
#include "lib/gc.h"
#include "lib/nullstream.h"

#include "backends/psa.h"
#include "backends/tna.h"
#include "backends/top.h"
#include "backends/v1model.h"
#include "options.h"
#include "scope.h"
#include "version.h"

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
    } else if (target == "psa") {
        PSA::generate_includes(ostream);
        program = PSA::gen();
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

    setup_gc_logging();
    setup_signals();

    // add a dummy context
    AutoCompileContext autoBludgeonContext(new CODEGEN::P4CContext);
    auto options = CODEGEN::BludgeonOptions();
    // we only handle P4_16 right now
    options.compiler_version = CODEGEN_VERSION_STRING;

    options.process(argc, argv);
    if (::errorCount() > 0) {
        exit(EXIT_FAILURE);
    }
    // use a default name if no specific output name is provided
    if (options.output_file == nullptr) {
        options.output_file = "out.p4";
    }
    // use a default name if no specific output name is provided
    if (options.arch == nullptr) {
        ::error("--arch is required. Possible options: v1model, tna, or top");
        options.usage();
        exit(EXIT_FAILURE);
    }

    auto ostream = openFile(options.output_file, false);
    if (ostream == nullptr) {
        ::error("must have --output [file]");
        exit(EXIT_FAILURE);
    }
    uint64_t seed;
    if (options.seed) {
        std::cerr << "Using provided seed.\n";
        try {
            seed = boost::lexical_cast<uint64_t>(options.seed);
        } catch (boost::bad_lexical_cast &) {
            ::error("invalid seed %s", options.seed);
            exit(EXIT_FAILURE);
        }
    } else {
        // no seed provided, we generate our own
        std::cerr << "Using generated seed.\n";
        std::random_device r;
        seed = r();
    }
    std::cerr << "Seed:" << seed << "\n";
    CODEGEN::set_seed(seed);

    CODEGEN::gen_p4_code(options.output_file, options.arch);

    return EXIT_SUCCESS;
}

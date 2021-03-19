#include "options.h"

namespace CODEGEN {

const char *defaultMessage = "Generate a P4 program";

BludgeonOptions::BludgeonOptions() {
    registerOption(
        "--output", "file",
        [this](const char *arg) {
            output_file = arg;
            return true;
        },
        "The name of the output file. Default is \"out.p4\".");
    registerOption(
        "--arch", "arch",
        [this](const char *arg) {
            arch = arg;
            return true;
        },
        "Generate code for the specified architecture.");
    registerOption(
        "--seed", "seed",
        [this](const char *arg) {
            seed = arg;
            return true;
        },
        "The seed for the random program. If no seed is provided we generate "
        "our own.");
}

} // namespace CODEGEN

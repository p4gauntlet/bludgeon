#include "options.h"

namespace CODEGEN {

const char *defaultMessage = "Generate a P4 program";

BludgeonOptions::BludgeonOptions() : Util::Options(defaultMessage) {
    registerOption(
        "--help", nullptr,
        [this](const char *) {
            usage();
            exit(0);
            return false;
        },
        "Print this help message");
    registerOption(
        "--version", nullptr,
        [this](const char *) {
            std::cerr << binaryName << std::endl;
            std::cerr << "Version " << compiler_version << std::endl;
            exit(0);
            return false;
        },
        "Print compiler version");
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
        "The seed for the random program. If no seed is provided we generate our own.");
}


} // namespace CODEGEN

#ifndef BLUDGEON_OPTIONS_H_
#define BLUDGEON_OPTIONS_H_

#include <getopt.h>
#include <unordered_set>

#include "lib/options.h"

namespace CODEGEN {

class BludgeonOptions : public Util::Options {

  public:
    BludgeonOptions();
    // name of the output file
    cstring output_file = nullptr;
    // which architecture to generate code for
    cstring arch = nullptr;
    // the current version of the compiler
    cstring compiler_version;
};

/// A compilation context which exposes compiler options.
class P4CContext : public BaseCompileContext {};


} // namespace CODEGEN

#endif /* BLUDGEON_OPTIONS_H_ */

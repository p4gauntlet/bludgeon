#ifndef BLUDGEON_OPTIONS_H_
#define BLUDGEON_OPTIONS_H_

#include <getopt.h>
#include <unordered_set>

#include "frontends/common/options.h"
#include "lib/options.h"

namespace CODEGEN {

class BludgeonOptions : public ParserOptions {

 public:
    BludgeonOptions();
    // name of the output file
    cstring output_file = nullptr;
    // which architecture to generate code for
    cstring arch = nullptr;
    // the current version of the compiler
    cstring compiler_version;
    cstring seed;
};

/// A compilation context which exposes compiler options.
using P4toZ3Context = P4CContextWithOptions<BludgeonOptions>;

} // namespace CODEGEN


#endif /* BLUDGEON_OPTIONS_H_ */

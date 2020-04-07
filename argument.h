#ifndef _ARGUMENT_H_
#define _ARGUMENT_H_

#include "ir/ir.h"

namespace CODEGEN {
class argument {
  public:
    const char *types[2] = {"expression", "DONTCARE"};

    argument() {}
};
} // namespace CODEGEN

#endif

#ifndef _INITIALIZER_H_
#define _INITIALIZER_H_

#include "ir/ir.h"

namespace CODEGEN {

class initializer {
  public:
    const char *types[1] = {"expression"};

    initializer() {}
};

} // namespace CODEGEN

#endif
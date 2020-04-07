#ifndef _TYPEARGUMENTLIST_H_
#define _TYPEARGUMENTLIST_H_

#include "ir/ir.h"

namespace CODEGEN {

class typeArgumentList {
  public:
    const char *types[1] = {"typeArg"};

    typeArgumentList() {}
};

} // namespace CODEGEN

#endif
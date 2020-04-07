#ifndef _REALTYPEARGUMENTLIST_H_
#define _REALTYPEARGUMENTLIST_H_

#include "ir/ir.h"

namespace CODEGEN {

class realTypeArgumentList {
  public:
    const char *types[1] = {"realTypeArg"};

    realTypeArgumentList() {}
};

} // namespace CODEGEN

#endif
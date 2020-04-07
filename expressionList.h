#ifndef _EXPRESSIONLIST_H_
#define _EXPRESSIONLIST_H_

#include "ir/ir.h"

namespace CODEGEN {

class expressionList {
  public:
    const char *types[1] = {"expression"};

    expressionList() {}
};

} // namespace CODEGEN

#endif
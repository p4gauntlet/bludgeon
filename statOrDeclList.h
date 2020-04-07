#ifndef _STATORDECLLIST_H_
#define _STATORDECLLIST_H_

#include "ir/ir.h"

namespace CODEGEN {

class statOrDeclList {
  public:
    const char *types[0] = {};

    statOrDeclList() {}
};

} // namespace CODEGEN

#endif
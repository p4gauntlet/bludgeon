#ifndef _KVLIST_H_
#define _KVLIST_H_

#include "ir/ir.h"

namespace CODEGEN {

class kvList {
  public:
    const char *types[1] = {"kvPair"};

    kvList() {}
};

} // namespace CODEGEN

#endif
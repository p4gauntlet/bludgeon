#ifndef _TUPLETYPE_H_
#define _TUPLETYPE_H_

#include "ir/ir.h"

namespace CODEGEN {

class tupleType {
  public:
    const char *types[0] = {};

    tupleType() {}
};

} // namespace CODEGEN

#endif

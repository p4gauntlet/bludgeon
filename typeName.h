#ifndef _TYPENAME_H_
#define _TYPENAME_H_

#include "ir/ir.h"

#include "prefixedType.h"

namespace CODEGEN {
class typeName {
  public:
    const char *types[1] = {"prefixedType"};

    typeName() {}

    static IR::Type *gen(cstring for_type);
};
} // namespace CODEGEN

#endif

#ifndef _HEADERSTACKTYPE_H_
#define _HEADERSTACKTYPE_H_

#include "ir/ir.h"

#include "expression.h"
#include "typeName.h"

#include "scope.h"

namespace CODEGEN {
class headerStackType {
  public:
    const char *types[0] = {};

    headerStackType() {}

    ~headerStackType() {}

    static IR::Type *gen(cstring for_type);
};
} // namespace CODEGEN

#endif

#ifndef _HEADERSTACKTYPE_H_
#define _HEADERSTACKTYPE_H_

#include "ir/ir.h"

#include "expression.h"
#include "typeName.h"

#include "scope.h"

#define MAX_HEADER_STACK_SIZE 10

namespace CODEGEN {
class headerStackType {
  public:
    const char *types[0] = {};

    headerStackType() {}

    ~headerStackType() {}

    static IR::Type *gen();
};
} // namespace CODEGEN

#endif

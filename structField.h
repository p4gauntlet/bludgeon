#ifndef _STRUCTFIELD_H_
#define _STRUCTFIELD_H_

#include "codegen.h"
#include "ir/ir.h"
#include "typeRef.h"

namespace CODEGEN {

class structField {
  public:
    const char *types[0] = {};

    structField() {}

    static IR::StructField *gen();
};

} // namespace CODEGEN

#endif

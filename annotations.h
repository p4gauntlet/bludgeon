#ifndef _ANNOTATIONS_H_
#define _ANNOTATIONS_H_

#include "codegen.h"
#include "expression.h"
#include "ir/ir.h"

namespace CODEGEN {
class Annotations {
  public:
    const char *types[0] = {};


    Annotations() { }

    static IR::Annotations *gen();
};
} // namespace CODEGEN

#endif

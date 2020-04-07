#ifndef _CONTROLTYPEDECLARATION_H_
#define _CONTROLTYPEDECLARATION_H_

#include "ir/ir.h"

#include "codegen.h"
#include "parameterList.h"

#include "scope.h"

namespace CODEGEN {
class controlTypeDeclaration {
  public:
    const char *types[0] = {};

    controlTypeDeclaration() {}

    ~controlTypeDeclaration() {}

    static IR::Type_Control *gen_ing_ctrl_type(int tf_flag);
};
} // namespace CODEGEN

#endif

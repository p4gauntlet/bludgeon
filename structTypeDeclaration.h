#ifndef _STRUCTTYPEDECLARATION_H_
#define _STRUCTTYPEDECLARATION_H_

#include "ir/ir.h"

#include "annotations.h"
#include "common.h"
#include "scope.h"
#include "structFieldList.h"

namespace CODEGEN {
class structTypeDeclaration {
  public:
    const char *types[0] = {};

    structTypeDeclaration() {}

    static IR::Type_Struct *gen();
    static IR::Type_Struct *gen_Headers();
};
} // namespace CODEGEN

#endif // ifndef _STRUCTTYPEDECLARATION_H_

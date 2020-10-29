#ifndef _STRUCTTYPEDECLARATION_H_
#define _STRUCTTYPEDECLARATION_H_

#include "ir/ir.h"

namespace CODEGEN {
class structTypeDeclaration {
 public:
    const char *types[0] = {};

    structTypeDeclaration() {}

    static IR::Type_Struct *gen();
    static IR::Type_Struct *gen_Headers();

 private:
    static IR::StructField *pick_field();
};
} // namespace CODEGEN

#endif // ifndef _STRUCTTYPEDECLARATION_H_

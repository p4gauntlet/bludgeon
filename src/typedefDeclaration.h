#ifndef _TYPEDEFDECLARATION_H_
#define _TYPEDEFDECLARATION_H_

#include "ir/ir.h"

#include "baseType.h"
#include "common.h"
#include "scope.h"
#include "typeRef.h"

#include "headerTypeDeclaration.h"
#include "headerUnionDeclaration.h"
#include "structTypeDeclaration.h"

namespace CODEGEN {

class typedefDeclaration {
 public:
    const char *types[2] = {"typedef", "type"};

    typedefDeclaration() {}
    ~typedefDeclaration() {}

    static IR::Type *gen();
    static void gen_base_t();
    static IR::Type_Typedef *gen_typedef();
    static IR::Type_Newtype *gen_newtype();
};

} // namespace CODEGEN

#endif

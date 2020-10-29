#ifndef _DERIVEDTYPEDECLARATION_H_
#define _DERIVEDTYPEDECLARATION_H_

#include "ir/ir.h"

namespace CODEGEN {

class derivedTypeDeclaration {
 public:
    const char *types[4] = {"headerTypeDeclaration", "headerUnionDeclaration",
                            "structTypeDeclaration", "enumDeclaration"};

    derivedTypeDeclaration() {}

    static IR::Type *gen();
};

} // namespace CODEGEN

#endif

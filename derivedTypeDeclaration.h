#ifndef _DERIVEDTYPEDECLARATION_H_
#define _DERIVEDTYPEDECLARATION_H_

#include "ir/ir.h"

#include "headerTypeDeclaration.h"

namespace CODEGEN {

class derivedTypeDeclaration {
  public:
    const char *types[4] = {"headerTypeDeclaration", "headerUnionDeclaration",
                            "structTypeDeclaration", "enumDeclaration"};

    derivedTypeDeclaration() {}

    static IR::Type *gen() {
        switch (rand() % 1) {
        case 0:
            return headerTypeDeclaration::gen();
        }
    }
};

} // namespace CODEGEN

#endif

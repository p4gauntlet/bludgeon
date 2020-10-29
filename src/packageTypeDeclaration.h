#ifndef _PACKAGETYPEDECLARATION_H_
#define _PACKAGETYPEDECLARATION_H_

#include "ir/ir.h"

namespace CODEGEN {

class packageTypeDeclaration {
 public:
    const char *types[0] = {};

    packageTypeDeclaration() {}
};

} // namespace CODEGEN

#endif
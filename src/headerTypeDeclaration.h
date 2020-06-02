#ifndef _HEADERTYPEDECLARATION_H_
#define _HEADERTYPEDECLARATION_H_

#include "ir/ir.h"

#include "scope.h"
#include "structFieldList.h"

#include "common.h"

namespace CODEGEN {

class headerTypeDeclaration {
  public:
    const char *types[0] = {};

    headerTypeDeclaration() {}

    static IR::Type_Header *gen();
    static IR::Type_Header *gen_eth();
};

} // namespace CODEGEN

#endif

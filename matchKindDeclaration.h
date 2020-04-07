#ifndef _MATCHKINDDECLARATION_H_
#define _MATCHKINDDECLARATION_H_

#include "ir/ir.h"

namespace CODEGEN {

class matchKindDeclaration {
  public:
    const char *types[1] = {"MATCH_KIND"};

    matchKindDeclaration() {}
};

} // namespace CODEGEN

#endif
#ifndef _CONTROLBODY_H_
#define _CONTROLBODY_H_

#include "ir/ir.h"

namespace CODEGEN {

class controlBody {
  public:
    const char *types[1] = {"blockStatement"};

    controlBody() {}
};

} // namespace CODEGEN

#endif
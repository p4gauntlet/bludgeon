#ifndef _REALTYPEARG_H_
#define _REALTYPEARG_H_

#include "ir/ir.h"

namespace CODEGEN {

class realTypeArg {
 public:
    const char *types[2] = {"DONTCARE", "typeRef"};

    realTypeArg() {}
};

} // namespace CODEGEN

#endif

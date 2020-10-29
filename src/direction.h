#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include "ir/ir.h"

namespace CODEGEN {

class direction {
 public:
    const char *types[3] = {"IN", "OUT", "INOUT"};

    direction() {}
};

} // namespace CODEGEN

#endif

#ifndef _EMPTYSTATEMENT_H_
#define _EMPTYSTATEMENT_H_

#include "ir/ir.h"

namespace CODEGEN {

class emptyStatement {
 public:
    const char *types[0] = {};

    emptyStatement() {}
};

} // namespace CODEGEN

#endif

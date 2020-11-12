#ifndef _EXPRESSIONLIST_H_
#define _EXPRESSIONLIST_H_

#include "ir/ir.h"

namespace CODEGEN {

class expressionList {
 public:
    const char *types[1] = {"expression"};

    expressionList() {}
    static IR::ListExpression *gen(IR::Vector<IR::Type> types,
                                   bool only_lval = false);
};

} // namespace CODEGEN

#endif

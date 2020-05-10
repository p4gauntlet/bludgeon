#ifndef _expression_bit_H_
#define _expression_bit_H_

#include "expression.h"
#include "ir/ir.h"

namespace CODEGEN {

class expression_bit {
  public:
    expression_bit() {}

    static IR::Expression *construct(const IR::Type_Bits *tb, Requirements *req,
                                     Properties *prop);
};
} // namespace CODEGEN

#endif

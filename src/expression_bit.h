#ifndef _expression_bit_H_
#define _expression_bit_H_

#include "expression.h"
#include "expression_boolean.h"
#include "expression_struct.h"
#include "ir/ir.h"

namespace CODEGEN {

class expression_bit {
    friend class expression_boolean;
    friend class expression_struct;
    friend class expression_int;
    friend class expression;

  public:
    expression_bit() {}

  protected:
    static IR::Expression *construct(const IR::Type_Bits *tb);
    static IR::Expression *construct_unary_expr(const IR::Type_Bits *tb);
    static IR::Expression *construct_binary_expr(const IR::Type_Bits *tb);
    static IR::Expression *construct_ternary_expr(const IR::Type_Bits *tb);
};
} // namespace CODEGEN

#endif

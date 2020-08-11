#ifndef _EXPRESSION_INT_H_
#define _EXPRESSION_INT_H_

#include "expression.h"
#include "expression_boolean.h"
#include "expression_struct.h"
#include "ir/ir.h"

namespace CODEGEN {

class expression_int {
    friend class expression_boolean;
    friend class expression_struct;
    friend class expression_bit;
    friend class expression;

  public:
    expression_int() {}

  protected:
    static IR::Expression *construct();
    static IR::Expression *construct_unary_expr();
    static IR::Expression *construct_binary_expr();
};
} // namespace CODEGEN

#endif

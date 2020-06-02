#ifndef _expression_boolean_H_
#define _expression_boolean_H_

#include "expression.h"
#include "expression_bit.h"
#include "expression_struct.h"

#include "ir/ir.h"

namespace CODEGEN {

class expression_boolean {

    friend class expression_bit;
    friend class expression_struct;
    friend class expression;

  public:
    expression_boolean() {}

  protected:
    static IR::Expression *construct();
    static IR::Expression *construct_cmp_expr();
};
} // namespace CODEGEN

#endif

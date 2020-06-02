#ifndef _expression_struct_H_
#define _expression_struct_H_

#include "expression.h"
#include "expression_bit.h"
#include "expression_boolean.h"

#include "ir/ir.h"

namespace CODEGEN {

class expression_struct {
    friend class expression_bit;
    friend class expression_boolean;
    friend class expression;

  public:
    expression_struct() {}

  protected:
    static IR::Expression *construct(const IR::Type_Name *tn);
};
} // namespace CODEGEN

#endif

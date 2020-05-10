#ifndef _expression_boolean_H_
#define _expression_boolean_H_

#include "expression.h"
#include "ir/ir.h"

namespace CODEGEN {

class expression_boolean {
  public:
    expression_boolean() {}

    static IR::Expression *construct(Requirements *req, Properties *prop);
};
} // namespace CODEGEN

#endif

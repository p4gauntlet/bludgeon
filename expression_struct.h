#ifndef _expression_struct_H_
#define _expression_struct_H_

#include "expression.h"
#include "ir/ir.h"

namespace CODEGEN {

class expression_struct {
  public:
    expression_struct() {}

    static IR::Expression *construct(const IR::Type_Name *tn, Requirements *req,
                                     Properties *prop);
};
} // namespace CODEGEN

#endif

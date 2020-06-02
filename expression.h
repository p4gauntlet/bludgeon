#ifndef _expression_H_
#define _expression_H_

#include "ir/ir.h"
#include "scope.h"

namespace CODEGEN {

#define MAX_DEPTH 3

class expression {

  public:
    expression() {}

    static IR::MethodCallExpression *gen_functioncall(cstring method_name,
                                                      IR::ParameterList params);
    static IR::Expression *gen_expr(const IR::Type *tp);

    static IR::MethodCallExpression *
    pick_function(IR::IndexedVector<IR::Declaration> viable_functions,
                  const IR::Type **ret_type);

};
} // namespace CODEGEN

#endif

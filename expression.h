#ifndef _expression_H_
#define _expression_H_

#include "ir/ir.h"
#include "scope.h"

namespace CODEGEN {

#define MAX_DEPTH 3

struct Requirements {
    bool require_scalar;
    bool compile_time_known;
    bool no_methodcalls;
    bool not_zero;
    Requirements()
        : require_scalar(false),
          compile_time_known(false), no_methodcalls{false}, not_zero(false){};
};

struct Properties {
    bool width_unknown;
    bool has_methodcall;
    size_t depth = 0;
    Properties() : width_unknown(false), has_methodcall{false} {}
};

class expression {
  public:
    expression() {}
    static IR::MethodCallExpression *gen_functioncall(cstring method_name,
                                                      IR::ParameterList params);
    static IR::Expression *gen_expr(const IR::Type *tp,
                                    Requirements *req = nullptr);

    template <typename T>
    static IR::MethodCallExpression *pick_function(Requirements *req);
};
} // namespace CODEGEN

#endif

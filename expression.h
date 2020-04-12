#ifndef _expression_H_
#define _expression_H_

#include "ir/ir.h"

#include "bit.h"
#include "bool.h"
#include "int.h"

#include "scope.h"

namespace CODEGEN {

struct Requirements {
    bool require_scalar;
    bool no_methodcalls;
    Requirements() : require_scalar(false), no_methodcalls{false} {};
};

class expression {
  public:
    expression() {}

    static IR::Expression *gen_expr(const IR::Type *tp,
                                    Requirements *req = nullptr);
    static IR::Expression *gen_input_arg(const IR::Parameter *param);
    static bool check_input_arg(const IR::Parameter *param);
};
} // namespace CODEGEN

#endif

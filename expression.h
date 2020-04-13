#ifndef _expression_H_
#define _expression_H_

#include "ir/ir.h"

namespace CODEGEN {

struct Requirements {
    bool require_scalar;
    bool compile_time_known;
    bool no_methodcalls;
    bool not_zero;
    Requirements()
        : require_scalar(false),
          compile_time_known(false), no_methodcalls{false}, not_zero(false){};
};

class expression {
  public:
    expression() {}

    static IR::Expression *gen_expr(const IR::Type *tp,
                                    Requirements *req = nullptr);
};
} // namespace CODEGEN

#endif

#ifndef _expression_H_
#define _expression_H_

#include "ir/ir.h"


#include "bool.h"
#include "bit.h"
#include "int.h"

#include "codegen.h"
#include "scope.h"


namespace CODEGEN {
class expression {
public:

    expression() {   }


    static IR::Expression *gen_expr(const IR::Type *tp,
                                    bool           require_width = false);
    static IR::Expression *gen_input_arg(const IR::Parameter *param);
    static bool check_input_arg(const IR::Parameter *param);
    static IR::Expression *gen_literal(int tp_literal);

};
} // namespace CODEGEN



#endif

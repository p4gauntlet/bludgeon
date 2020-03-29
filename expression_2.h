#ifndef _EXPRESSION2_H_
#define _EXPRESSION2_H_

#include "ir/ir.h"


#include "bool.h"
#include "bit.h"
#include "int.h"

#include "codegen.h"
#include "scope.h"
#include "expression.h"


namespace CODEGEN {
class expression2 {
public:

    expression2() {   }


    static IR::Expression *gen_expr(const IR::Type *tp);
    static IR::Expression *gen_input_arg(const IR::Parameter *param);

};
} // namespace CODEGEN



#endif

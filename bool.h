#ifndef _BOOL_H_
#define _BOOL_H_

#include "ir/ir.h"


namespace CODEGEN {
class bool_literal {
public:
    bool_literal() {
    }

    static IR::Type_Boolean *gen() {
        return new IR::Type_Boolean();
    }

    static IR::BoolLiteral *gen_literal() {
        if (rand() % 2 == 0) {
            return new IR::BoolLiteral(false);
        } else {
            return new IR::BoolLiteral(true);
        }
    }
};
} // namespace CODEGEN



#endif

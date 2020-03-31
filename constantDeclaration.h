#ifndef _CONSTANTDECLARATION_H_
#define _CONSTANTDECLARATION_H_

#include "ir/ir.h"

#include "codegen.h"
#include "scope.h"
#include "expression.h"
#include "baseType.h"

namespace CODEGEN {
class constantDeclaration {
public:
    const char *types[0] = {
    };

    IR::ID *name;
    IR::Expression *expr;
    IR::Type *tp;

    constantDeclaration() {
        name = new IR::ID(CODEGEN::randstr(4));
    }

    ~constantDeclaration() {
        delete name;
        delete expr;
        delete tp;
    }

    IR::Declaration_Constant *gen() {
        std::vector<int> type = { 0, 1 };
        auto base_type        = new baseType(false, type);

        tp = base_type->gen();

        if (tp->is<IR::Type_Boolean>()) {
            expr = new IR::BoolLiteral(false);
        } else {
            expr = new IR::Constant(tp->to<IR::Type_Bits>(), rand() % 8 + 1);
        }

        // add to the scope
        auto ret = new IR::Declaration_Constant(*name, tp, expr);
        P4Scope::add_to_scope(ret);
        P4Scope::add_name_2_type_c(name->name, tp);
        return ret;
    }
};
} // namespace CODEGEN



#endif

#ifndef _VARIABLEDECLARATION_H_
#define _VARIABLEDECLARATION_H_

#include "ir/ir.h"


#include "common.h"
#include "codegen.h"
#include "scope.h"
#include "expression.h"
#include "expression_2.h"


#include "typeRef.h"

namespace CODEGEN {
class variableDeclaration {
public:
    const char *types[0] = {
    };

    IR::ID *name                  = nullptr;
    IR::Type *type                = nullptr;
    IR::Expression *expr          = nullptr;
    IR::ListExpression *list_expr = nullptr;


    variableDeclaration() {
        name = new IR::ID(CODEGEN::randstr(6));
    }

    ~variableDeclaration() {
        delete name;
        delete type;
        if (expr != nullptr) {
            delete expr;
        }
        if (list_expr != nullptr) {
            delete list_expr;
        }
    }

    IR::Declaration_Variable *gen();
};
} // namespace CODEGEN



#endif

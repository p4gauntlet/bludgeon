#ifndef _VARIABLEDECLARATION_H_
#define _VARIABLEDECLARATION_H_

#include "ir/ir.h"


#include "common.h"
#include "codegen.h"
#include "scope.h"
#include "expression.h"


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

    IR::Declaration_Variable *gen() {
        std::vector<int> types = {};
        typeRef *type_ref      = new typeRef(true, types, STRUCT_LIKE);

        type = type_ref->gen();

        IR::Declaration_Variable *ret;
        // Tao: construct list expression
        if (type->is<IR::Type_Bits>()) {
            auto tp_bits = type->to<IR::Type_Bits>();
            auto size    = tp_bits->size >=
                           SIZE_BIT_FOR_INITIALIZATION ?
                           SIZE_BIT_FOR_INITIALIZATION :
                           tp_bits->size;
            auto rand_num = rand() % (1 << size);
            ret = new IR::Declaration_Variable(*name, type,
                                               new IR::Constant(tp_bits,
                                                                rand_num));
        } else if (type->is<IR::Type_Boolean>()) {
            if (rand() % 2 == 0) {
                ret = new IR::Declaration_Variable(*name, type, new IR::BoolLiteral(
                                                       false));
            } else{
                ret = new IR::Declaration_Variable(*name, type, new IR::BoolLiteral(
                                                       true));
            }
        } else{
            IR::Vector<IR::Expression> exprs;
            bool if_contains_stack = false;
            expression::construct_list_expr(type, exprs, &if_contains_stack);
            bool is_forbidden = false;
            if (type->is<IR::Type_Name>()) {
                auto tp_n = type->to<IR::Type_Name>();
                if (P4Scope::not_initialized_structs.find(tp_n->path->name.name)
                    != P4Scope::not_initialized_structs.end()) {
                    is_forbidden = true;
                }
            }
            if ((if_contains_stack == false) && (is_forbidden == false)) {
                // ret = new IR::Declaration_Variable(*name, type, new IR::ListExpression(exprs));
                ret = new IR::Declaration_Variable(*name, type, exprs.at(0));
            } else{
                ret = new IR::Declaration_Variable(*name, type);
                // Tao: this needs further initialization
                P4Scope::add_name_2_type_stack(name->name, type);
            }
        }

        //
        P4Scope::add_to_scope((IR::Node *)ret);
        P4Scope::add_lval(type, name->name);
        P4Scope::add_name_2_type_v(name->name, type);

        return ret;
    }
};
} // namespace CODEGEN



#endif

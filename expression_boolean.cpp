#include "expression_boolean.h"

#include "argument.h"
#include "baseType.h"
#include "expression_bit.h"
#include "scope.h"

namespace CODEGEN {

IR::Expression *construct_cmp_expr(Requirements *req, Properties *prop) {
    IR::Expression *expr = nullptr;

    // gen some random type
    // can be either bits, int, bool, or structlike
    // for now it is just bits
    auto new_type_size = rand() % 128 + 1;
    auto new_type = new IR::Type_Bits(new_type_size, false);
    IR::Expression *left = expression_bit::construct(new_type, req, prop);
    IR::Expression *right = expression_bit::construct(new_type, req, prop);

    std::vector<int64_t> percent = {50, 50};

    switch (randind(percent)) {
    case 0: {
        expr = new IR::Equ(left, right);
        // pick an equals that matches the type
    } break;
    case 1: {
        expr = new IR::Neq(left, right);
        // pick a not-equals that matches the type
    } break;
    }
    return expr;
}

IR::Expression *expression_boolean::construct(Requirements *req,
                                              Properties *prop) {
    IR::Expression *expr = nullptr;
    IR::Expression *left;
    IR::Expression *right;

    std::vector<int64_t> percent = {15, 20, 40, 5, 5, 10, 5};

    switch (randind(percent)) {
    case 0: {
        auto tb = new IR::Type_Boolean();
        // TODO: This is lazy, we can easily check
        if (req->compile_time_known) {
            expr = baseType::gen_bool_literal();
            break;
        }
        if (P4Scope::check_lval(tb)) {
            cstring name = P4Scope::pick_lval(tb);
            expr = new IR::TypeNameExpression(name);
        } else {
            expr = baseType::gen_bool_literal();
        }
    } break;
    case 1: {
        // pick a boolean literal
        expr = baseType::gen_bool_literal();
    } break;
    case 2: {
        // pick a Not expression
        expr = new IR::LNot(construct(req, prop));
    } break;
    case 3: {
        // pick an And expression
        left = construct(req, prop);
        right = construct(req, prop);
        expr = new IR::LAnd(left, right);
    } break;
    case 4: {
        // pick an Or expression
        left = construct(req, prop);
        right = construct(req, prop);
        expr = new IR::LOr(left, right);
    } break;
    case 5: {
        // pick a comparison
        expr = construct_cmp_expr(req, prop);
    } break;
    case 6: {
        // run a function call
        auto p4_functions = P4Scope::get_decls<IR::Function>();

        IR::IndexedVector<IR::Declaration> viable_functions;
        for (auto fun : p4_functions) {
            if (fun->type->returnType->to<IR::Type_Boolean>()) {
                viable_functions.push_back(fun);
            }
        }

        // TODO: Make this more sophisticated
        if (viable_functions.size() == 0 || req->compile_time_known) {
            expr = baseType::gen_bool_literal();
            break;
        }

        size_t idx = rand() % viable_functions.size();
        auto p4_fun = viable_functions[idx]->to<IR::Function>();
        cstring fun_name = p4_fun->name.name;
        auto params = p4_fun->getParameters();
        auto ret_type = p4_fun->type->returnType;
        expr = expression::gen_functioncall(fun_name, *params);
        // sometimes, functions may not be callable
        // because we do not have the right return values
        if (not expr || not ret_type) {
            expr = baseType::gen_bool_literal();
            break;
        }
    }
    }
    return expr;
}

} // namespace CODEGEN

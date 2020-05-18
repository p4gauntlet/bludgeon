#include "expression.h"

#include "argument.h"
#include "baseType.h"
#include "expression_bit.h"
#include "expression_boolean.h"
#include "expression_struct.h"

namespace CODEGEN {

IR::MethodCallExpression *
expression::gen_functioncall(cstring method_name, IR::ParameterList params) {
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
    IR::IndexedVector<IR::StatOrDecl> decls;

    bool can_call = true;

    for (auto par : params) {
        if (not argument::check_input_arg(par)) {
            can_call = false;
        } else {
            IR::Argument *arg;
            arg = new IR::Argument(argument::gen_input_arg(par));
            args->push_back(arg);
        }
    }
    if (can_call) {
        auto path_expr = new IR::PathExpression(method_name);
        return new IR::MethodCallExpression(path_expr, args);
    } else {
        return nullptr;
    }
}

template <typename T>
IR::MethodCallExpression *expression::pick_function(Requirements *req) {
    auto p4_functions = P4Scope::get_decls<IR::Function>();

    IR::IndexedVector<IR::Declaration> viable_functions;
    for (auto fun : p4_functions) {
        if (fun->type->returnType->to<T>()) {
            viable_functions.push_back(fun);
        }
    }

    // TODO: Make this more sophisticated
    if (viable_functions.size() == 0 || req->compile_time_known) {
        return nullptr;
    }

    size_t idx = rand() % viable_functions.size();
    auto p4_fun = viable_functions[idx]->to<IR::Function>();
    cstring fun_name = p4_fun->name.name;
    auto params = p4_fun->getParameters();
    // sometimes, functions may not be callable
    // because we do not have the right return values
    return expression::gen_functioncall(fun_name, *params);
}

IR::Expression *expression::gen_expr(const IR::Type *tp, Requirements *req) {
    IR::Expression *expr = nullptr;

    Properties *prop = new Properties();

    // if no specific requirements were passed just generate default settings
    if (not req) {
        req = new Requirements();
    }

    // TODO: Add specific restrictions to types later
    if (auto tb = tp->to<IR::Type_Bits>()) {
        expr = expression_bit::construct(tb, req, prop);
    } else if (tp->is<IR::Type_InfInt>()) {
        big_int max_size = ((big_int)1U << 32);
        expr = baseType::gen_int_literal(max_size, req->not_zero);
    } else if (tp->is<IR::Type_Boolean>()) {
        expr = expression_boolean::construct(req, prop);
    } else if (auto tn = tp->to<IR::Type_Name>()) {
        expr = expression_struct::construct(tn, req, prop);
    } else {
        BUG("Expression: Type %s not yet supported", tp->node_type_name());
    }
    return expr;
}

} // namespace CODEGEN

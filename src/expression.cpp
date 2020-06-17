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

IR::MethodCallExpression *
expression::pick_function(IR::IndexedVector<IR::Declaration> viable_functions,
                          const IR::Type **ret_type) {
    // TODO: Make this more sophisticated
    if (viable_functions.size() == 0 || P4Scope::req.compile_time_known) {
        return nullptr;
    }

    size_t idx = get_rnd_int(0, viable_functions.size() - 1);
    cstring fun_name;
    const IR::ParameterList *params;
    if (auto p4_fun = viable_functions[idx]->to<IR::Function>()) {
        fun_name = p4_fun->name.name;
        params = p4_fun->getParameters();
        *ret_type = p4_fun->type->returnType;
    } else if (auto p4_extern = viable_functions[idx]->to<IR::Method>()) {
        fun_name = p4_extern->name.name;
        params = p4_extern->getParameters();
        *ret_type = p4_extern->type->returnType;
    } else {
        BUG("Unknown callable: Type %s not yet supported",
            viable_functions[idx]->node_type_name());
    }
    auto expr = expression::gen_functioncall(fun_name, *params);
    // sometimes, functions may not be callable
    // because we do not have the right return values
    if (not expr || not ret_type) {
        return nullptr;
    }
    return expr;
}

IR::Expression *expression::gen_expr(const IR::Type *tp) {
    IR::Expression *expr = nullptr;

    // reset the expression depth
    P4Scope::prop.depth = 0;

    if (auto tb = tp->to<IR::Type_Bits>()) {
        expr = expression_bit::construct(tb);
    } else if (tp->is<IR::Type_InfInt>()) {
        expr = baseType::gen_int_literal(INTEGER_WIDTH);
    } else if (tp->is<IR::Type_Boolean>()) {
        expr = expression_boolean::construct();
    } else if (auto tn = tp->to<IR::Type_Name>()) {
        expr = expression_struct::construct(tn);
    } else {
        BUG("Expression: Type %s not yet supported", tp->node_type_name());
    }
    // reset the expression depth, just to be safe...
    P4Scope::prop.depth = 0;
    return expr;
}

} // namespace CODEGEN

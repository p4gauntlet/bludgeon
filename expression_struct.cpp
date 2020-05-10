#include "expression_struct.h"

#include "argument.h"
#include "baseType.h"
#include "expression_bit.h"
#include "scope.h"

namespace CODEGEN {

IR::ListExpression *gen_struct_list(const IR::Type_Name *tn, Requirements *req,
                                    Properties *prop) {
    IR::Vector<IR::Expression> components;
    cstring tn_name = tn->path->name.name;

    if (auto td = P4Scope::get_type_by_name(tn_name)) {
        if (auto tn_type = td->to<IR::Type_StructLike>()) {
            for (auto sf : tn_type->fields) {
                IR::Expression *expr;
                if (auto field_tb = sf->type->to<IR::Type_Bits>()) {
                    expr = expression_bit::construct(field_tb, req, prop);
                } else if (auto field_tn = sf->type->to<IR::Type_Name>()) {
                    expr = gen_struct_list(field_tn, req, prop);
                } else {
                    BUG("gen_struct_list: Type %s not yet supported",
                        sf->type->node_type_name());
                }
                components.push_back(expr);
            }
        } else {
            BUG("gen_struct_list: Requested Type %s not a struct-like type",
                tn_name);
        }
    } else {
        BUG("gen_struct_list: Requested Type %s not found", tn_name);
    }
    return new IR::ListExpression(components);
}

IR::Expression *expression_struct::construct(const IR::Type_Name *tn,
                                             Requirements *req,
                                             Properties *prop) {
    IR::Expression *expr = nullptr;
    std::vector<int64_t> percent = {50, 30, 20};

    // because fallthrough is not very portable...
    bool use_default_expr = false;

    switch (randind(percent)) {
    case 0:
        // pick a type from the available list
        if (P4Scope::check_lval(tn)) {
            cstring lval = P4Scope::pick_lval(tn);
            expr = new IR::TypeNameExpression(lval);
        } else {
            // if there is no suitable declaration we fall through
            use_default_expr = true;
        }
        break;
    case 1: {
        // construct a list expression out of base-types
        use_default_expr = true;
    } break;
    case 2: {
        // run a function call
        auto p4_functions = P4Scope::get_decls<IR::Function>();

        IR::IndexedVector<IR::Declaration> viable_functions;
        for (auto fun : p4_functions) {
            if (fun->type->returnType == tn) {
                viable_functions.push_back(fun);
            }
        }
        // TODO: Make this more sophisticated
        if (viable_functions.size() == 0 || req->compile_time_known) {
            use_default_expr = true;
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
            use_default_expr = true;
            break;
        }
    }
    }
    if (use_default_expr) {
        expr = gen_struct_list(tn, req, prop);
    }
    return expr;
}

} // namespace CODEGEN

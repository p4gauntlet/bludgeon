#include "expression_struct.h"

#include "argument.h"
#include "baseType.h"
#include "scope.h"

namespace CODEGEN {

IR::ListExpression *gen_struct_list(const IR::Type_Name *tn) {
    IR::Vector<IR::Expression> components;
    cstring tn_name = tn->path->name.name;

    if (auto td = P4Scope::get_type_by_name(tn_name)) {
        if (auto tn_type = td->to<IR::Type_StructLike>()) {
            for (auto sf : tn_type->fields) {
                IR::Expression *expr;
                if (auto field_tn = sf->type->to<IR::Type_Name>()) {
                    // cannot use another type here yet
                    expr = gen_struct_list(field_tn);
                    components.push_back(expr);
                } else if (auto field_ts = sf->type->to<IR::Type_Stack>()) {
                    auto stack_size = field_ts->getSize();
                    auto stack_type = field_ts->elementType;
                    if (auto s_type_name = stack_type->to<IR::Type_Name>()) {
                        for (size_t idx = 0; idx < stack_size; ++idx) {
                            expr = gen_struct_list(s_type_name);
                            components.push_back(expr);
                        }

                    } else {
                        BUG("gen_struct_list: Stack Type %s unsupported",
                            tn_name);
                    }
                } else {
                    expr = expression::gen_expr(sf->type);
                    components.push_back(expr);
                }
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

IR::Expression *expression_struct::construct(const IR::Type_Name *tn) {
    IR::Expression *expr = nullptr;
    std::vector<int64_t> percent = {PCT.EXPRESSION_STRUCT_VAR,
                                    PCT.EXPRESSION_STRUCT_LITERAL,
                                    PCT.EXPRESSION_STRUCT_FUNCTION};

    // because fallthrough is not very portable...
    bool use_default_expr = false;

    switch (randind(percent)) {
    case 0:
        // pick a type from the available list
        // do not pick, if the requirement is to be a compile time known value
        if (P4Scope::check_lval(tn) and not P4Scope::req.compile_time_known) {
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
        auto p4_externs = P4Scope::get_decls<IR::Method>();

        IR::IndexedVector<IR::Declaration> viable_functions;
        for (auto fun : p4_functions) {
            if (fun->type->returnType == tn) {
                viable_functions.push_back(fun);
            }
        }
        for (auto fun : p4_externs) {
            if (fun->type->returnType == tn) {
                viable_functions.push_back(fun);
            }
        }

        const IR::Type *ret_type;
        expr = expression::pick_function(viable_functions, &ret_type);
        // can not find a suitable function, generate a default value
        if (not expr) {
            use_default_expr = true;
            break;
        }
    }
    }
    if (use_default_expr) {
        expr = gen_struct_list(tn);
    }
    return expr;
}

} // namespace CODEGEN

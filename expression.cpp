#include "expression.h"

#include "argument.h"
#include "baseType.h"
#include "scope.h"

namespace CODEGEN {
#define MAX_DEPTH 3

struct Properties {
    bool width_unknown;
    bool has_methodcall;
    size_t depth = 0;
    Properties() : width_unknown(false), has_methodcall{false} {}
};

IR::Expression *construct_bit_expr(const IR::Type_Bits *tb, Requirements *req,
                                   Properties *prop);
IR::Expression *construct_boolean_expr(Requirements *req, Properties *prop);

IR::MethodCallExpression *
gen_functioncall_expression(cstring method_name, IR::ParameterList params) {
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

IR::Expression *construct_unary_expr(const IR::Type_Bits *tb, Requirements *req,
                                     Properties *prop) {
    IR::Expression *expr = nullptr;

    if (prop->depth > MAX_DEPTH) {
        return baseType::gen_bit_literal(tb);
    }
    prop->depth++;

    std::vector<int64_t> percent = {20, 20, 10, 50};

    switch (randind(percent)) {
    case 0: {
        // pick a negation that matches the type
        expr = new IR::Neg(tb, construct_bit_expr(tb, req, prop));
    } break;
    case 1: {
        // pick a complement that matches the type
        // width must be known so we cast
        expr = construct_bit_expr(tb, req, prop);
        if (prop->width_unknown) {
            expr = new IR::Cast(tb, expr);
            prop->width_unknown = false;
        }
        expr = new IR::Cmpl(tb, expr);
    } break;
    case 2: {
        // pick a cast to the type that matches the type
        // new bit type can be random here
        expr = new IR::Cast(tb, construct_bit_expr(tb, req, prop));
    } break;
    case 3: {
        auto p4_functions = P4Scope::get_decls<IR::Function>();
        // TODO: Make this more sophisticated
        if (p4_functions.size() == 0 || req->compile_time_known) {
            expr = baseType::gen_bit_literal(tb);
            break;
        }
        size_t idx = rand() % p4_functions.size();
        auto p4_fun = p4_functions[idx];
        cstring fun_name = p4_fun->name.name;
        auto params = p4_fun->getParameters();
        auto ret_type = p4_fun->type->returnType;
        expr = gen_functioncall_expression(fun_name, *params);
        // sometimes, functions may not be callable
        // because we do not have the right return values
        if (not expr || not ret_type) {
            expr = baseType::gen_bit_literal(tb);
            break;
        }
        // if the return value does not match try to cast it
        if (ret_type != tb) {
            expr = new IR::Cast(tb, expr);
        }
    } break;
    }
    return expr;
}

IR::Expression *construct_binary_expr(const IR::Type_Bits *tb,
                                      Requirements *req, Properties *prop) {
    IR::Expression *expr = nullptr;

    if (prop->depth > MAX_DEPTH) {
        return baseType::gen_bit_literal(tb);
    }
    prop->depth++;
    // todo add a restricted methodcallexpression here
    std::vector<int64_t> percent = {5, 5, 5, 10, 10, 10, 10, 5, 5, 10, 10, 10, 5};

    switch (randind(percent)) {
    case 0: {
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        // pick a multiplication that matches the type
        expr = new IR::Mul(tb, left, right);
    } break;
    case 1: {
        // pick a division that matches the type
        // TODO: Make more sophisticated
        // this requires only compile time known values
        IR::Expression *left = baseType::gen_bit_literal(tb, req->not_zero);
        req->not_zero = true;
        IR::Expression *right = baseType::gen_bit_literal(tb, req->not_zero);
        req->not_zero = false;
        expr = new IR::Div(tb, left, right);
    } break;
    case 2: {
        // pick a modulo that matches the type
        // TODO: Make more sophisticated
        // this requires only compile time known values
        IR::Expression *left = baseType::gen_bit_literal(tb, req->not_zero);
        req->not_zero = true;
        IR::Expression *right = baseType::gen_bit_literal(tb, req->not_zero);
        req->not_zero = false;
        expr = new IR::Mod(tb, left, right);
    } break;
    case 3: {
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        // pick an addition that matches the type
        expr = new IR::Add(tb, left, right);
    } break;
    case 4: {
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        // pick a subtraction that matches the type
        expr = new IR::Sub(tb, left, right);
    } break;
    case 5: {
        // width must be known so we cast
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        if (prop->width_unknown) {
            left = new IR::Cast(tb, left);
            prop->width_unknown = false;
        }
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        if (prop->width_unknown) {
            right = new IR::Cast(tb, right);
            prop->width_unknown = false;
        }
        // pick a saturating addition that matches the type
        expr = new IR::AddSat(tb, left, right);
    } break;
    case 6: {
        // width must be known so we cast
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        if (prop->width_unknown) {
            left = new IR::Cast(tb, left);
            prop->width_unknown = false;
        }
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        if (prop->width_unknown) {
            right = new IR::Cast(tb, right);
            prop->width_unknown = false;
        }
        // pick a saturating addition that matches the type
        expr = new IR::SubSat(tb, left, right);
    } break;
    case 7: {
        // width must be known so we cast
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        if (prop->width_unknown) {
            left = new IR::Cast(tb, left);
            prop->width_unknown = false;
        }
        // TODO: Make this more sophisticated,
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        // TODO: Make this more sophisticated
        // shifts are limited to 8 bits
        if (tb->width_bits() > 8) {
            right = new IR::Cast(new IR::Type_Bits(8, false), right);
        }
        // pick a left-shift that matches the type
        expr = new IR::Shl(tb, left, right);
    } break;
    case 8: {
        // width must be known so we cast
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        if (prop->width_unknown) {
            left = new IR::Cast(tb, left);
            prop->width_unknown = false;
        }

        // TODO: Make this more sophisticated,
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        // shifts are limited to 8 bits
        if (tb->width_bits() > 8) {
            right = new IR::Cast(new IR::Type_Bits(8, false), right);
        }
        // pick a right-shift that matches the type
        expr = new IR::Shr(tb, left, right);
    } break;
    case 9: {
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        // pick an binary And that matches the type
        expr = new IR::BAnd(tb, left, right);
    } break;
    case 10: {
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        // pick a binary Or and that matches the type
        expr = new IR::BOr(tb, left, right);
    } break;
    case 11: {
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        // pick an binary Xor that matches the type
        expr = new IR::BXor(tb, left, right);
    } break;
    case 12: {
        // pick an concatenation that matches the type
        size_t type_width = tb->width_bits();
        size_t split = (rand() % type_width) + 1;
        // TODO: lazy fallback
        if (split >= type_width) {
            return baseType::gen_bit_literal(tb);
        }
        auto tl = new IR::Type_Bits(type_width - split, false);
        auto tr = new IR::Type_Bits(split, false);
        // width must be known so we cast
        // width must be known so we cast
        IR::Expression *left = construct_bit_expr(tl, req, prop);
        if (prop->width_unknown) {
            left = new IR::Cast(tl, left);
            prop->width_unknown = false;
        }
        IR::Expression *right = construct_bit_expr(tr, req, prop);
        if (prop->width_unknown) {
            right = new IR::Cast(tr, right);
            prop->width_unknown = false;
        }
        expr = new IR::Concat(tb, left, right);
    } break;
    }
    return expr;
}

IR::Expression *construct_ternary_expr(const IR::Type_Bits *tb,
                                       Requirements *req, Properties *prop) {
    IR::Expression *expr = nullptr;

    if (prop->depth > MAX_DEPTH) {
        return baseType::gen_bit_literal(tb);
    }
    prop->depth++;
    std::vector<int64_t> percent = {50, 50};

    switch (randind(percent)) {
    case 0: {
        // TODO: Refine this...
        // pick a slice that matches the type
        auto type_width = tb->width_bits();
        // TODO this is some arbitrary value...
        auto new_type_size = rand() % 128 + type_width + 1;
        auto slice_type = new IR::Type_Bits(new_type_size, false);
        auto slice_expr = construct_bit_expr(slice_type, req, prop);
        if (prop->width_unknown) {
            slice_expr = new IR::Cast(slice_type, slice_expr);
            prop->width_unknown = false;
        }
        auto margin = new_type_size - type_width;
        size_t high = (rand() % margin) + type_width;
        size_t low = high - type_width + 1;
        expr = new IR::Slice(slice_expr, high, low);
        break;
    }
    case 1: {
        // pick a mux that matches the type
        IR::Expression *cond = construct_boolean_expr(req, prop);
        IR::Expression *left = construct_bit_expr(tb, req, prop);
        if (prop->width_unknown) {
            left = new IR::Cast(tb, left);
            prop->width_unknown = false;
        }
        IR::Expression *right = construct_bit_expr(tb, req, prop);
        if (prop->width_unknown) {
            right = new IR::Cast(tb, right);
            prop->width_unknown = false;
        }
        expr = new IR::Mux(tb, cond, left, right);
    } break;
    }
    return expr;
}

IR::Expression *pick_var(const IR::Type_Bits *tb) {
    cstring node_name = tb->node_type_name();
    auto avail_bit_types = P4Scope::lval_map[node_name].size();
    if (P4Scope::check_lval(tb)) {
        cstring name = P4Scope::pick_lval(tb);
        return new IR::PathExpression(name);
    } else if (avail_bit_types > 0) {
        // even if we do not find anything we can still cast other bits
        auto new_tb = P4Scope::pick_declared_bit_type();
        cstring name = P4Scope::pick_lval(new_tb);
        return new IR::Cast(tb, new IR::PathExpression(name));
    }

    // fallback, just generate a literal
    return baseType::gen_bit_literal(tb);
}

IR::Expression *construct_bit_expr(const IR::Type_Bits *tb, Requirements *req,
                                   Properties *prop) {
    IR::Expression *expr = nullptr;
    std::vector<int64_t> percent = {20, 5, 25, 10, 20, 10};

    switch (randind(percent)) {
    case 0: {
        // pick a variable that matches the type
        // do not pick, if the requirement is to be a compile time known value
        // TODO: This is lazy, we can easily check
        if (req->compile_time_known) {
            expr = baseType::gen_bit_literal(tb, req->not_zero);
        } else {
            expr = pick_var(tb);
        }
    } break;
    case 1: {
        // pick an int literal, if allowed
        if (req->require_scalar) {
            expr = baseType::gen_bit_literal(tb, req->not_zero);
        } else {
            bool has_int = P4Scope::check_lval(new IR::Type_InfInt());
            if (rand() % 2 || not has_int) {
                big_int max_size = ((big_int)1U << tb->width_bits());
                expr = baseType::gen_int_literal(max_size, req->not_zero);
            } else {
                cstring name = P4Scope::pick_lval(new IR::Type_InfInt());
                expr = new IR::PathExpression(name);
            }
            prop->width_unknown = true;
        }
    } break;
    case 2: {
        // pick a bit literal that matches the type
        expr = baseType::gen_bit_literal(tb, req->not_zero);
    } break;
    case 3: {
        // pick a unary expression that matches the type
        expr = construct_unary_expr(tb, req, prop);
    } break;
    case 4: {
        // pick a binary expression that matches the type
        expr = construct_binary_expr(tb, req, prop);
    } break;
    case 5: {
        // pick a ternary expression that matches the type
        expr = construct_ternary_expr(tb, req, prop);
    } break;
    }
    return expr;
}

IR::Expression *construct_cmp_expr(Requirements *req, Properties *prop) {
    IR::Expression *expr = nullptr;

    // gen some random type
    // can be either bits, int, bool, or structlike
    // for now it is just bits
    auto new_type_size = rand() % 128 + 1;
    auto new_type = new IR::Type_Bits(new_type_size, false);
    IR::Expression *left = construct_bit_expr(new_type, req, prop);
    IR::Expression *right = construct_bit_expr(new_type, req, prop);

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

IR::Expression *construct_boolean_expr(Requirements *req, Properties *prop) {
    IR::Expression *expr = nullptr;
    IR::Expression *left;
    IR::Expression *right;

    std::vector<int64_t> percent = {20, 20, 40, 5, 5, 10};

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
        expr = new IR::LNot(construct_boolean_expr(req, prop));
    } break;
    case 3: {
        // pick an And expression
        left = construct_boolean_expr(req, prop);
        right = construct_boolean_expr(req, prop);
        expr = new IR::LAnd(left, right);
    } break;
    case 4: {
        // pick an Or expression
        left = construct_boolean_expr(req, prop);
        right = construct_boolean_expr(req, prop);
        expr = new IR::LOr(left, right);
    } break;
    case 5: {
        // pick a comparison
        expr = construct_cmp_expr(req, prop);
    } break;
    }
    return expr;
}

IR::ListExpression *gen_struct_list(const IR::Type_Name *tn, Requirements *req,
                                    Properties *prop) {
    IR::Vector<IR::Expression> components;
    cstring tn_name = tn->path->name.name;

    if (auto td = P4Scope::get_type_by_name(tn_name)) {
        if (auto tn_type = td->to<IR::Type_StructLike>()) {
            for (auto sf : tn_type->fields) {
                IR::Expression *expr;
                if (auto field_tb = sf->type->to<IR::Type_Bits>()) {
                    expr = construct_bit_expr(field_tb, req, prop);
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

IR::Expression *construct_structlike_expr(const IR::Type_Name *tn,
                                          Requirements *req, Properties *prop) {
    IR::Expression *expr = nullptr;
    std::vector<int64_t> percent = {50, 50};

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
    }
    if (use_default_expr) {
        expr = gen_struct_list(tn, req, prop);
    }
    return expr;
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
        expr = construct_bit_expr(tb, req, prop);
    } else if (tp->is<IR::Type_InfInt>()) {
        big_int max_size = ((big_int)1U << 32);
        expr = baseType::gen_int_literal(max_size, req->not_zero);
    } else if (tp->is<IR::Type_Boolean>()) {
        expr = construct_boolean_expr(req, prop);
    } else if (auto tn = tp->to<IR::Type_Name>()) {
        expr = construct_structlike_expr(tn, req, prop);
    } else {
        BUG("Type %s not yet supported", tp->node_type_name());
    }
    return expr;
}

} // namespace CODEGEN

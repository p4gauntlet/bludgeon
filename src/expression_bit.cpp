#include "expression_bit.h"

#include "argument.h"
#include "baseType.h"
#include "scope.h"

namespace CODEGEN {

IR::Expression *expression_bit::construct_unary_expr(const IR::Type_Bits *tb) {
    IR::Expression *expr = nullptr;

    if (P4Scope::prop.depth > MAX_DEPTH) {
        return baseType::gen_bit_literal(tb);
    }
    P4Scope::prop.depth++;

    std::vector<int64_t> percent = {
        PCT.EXPRESSION_BIT_UNARY_NEG, PCT.EXPRESSION_BIT_UNARY_CMPL,
        PCT.EXPRESSION_BIT_UNARY_CAST, PCT.EXPRESSION_BIT_UNARY_FUNCTION};

    switch (randind(percent)) {
    case 0: {
        // pick a negation that matches the type
        expr = new IR::Neg(tb, expression_bit::construct(tb));
    } break;
    case 1: {
        // pick a complement that matches the type
        // width must be known so we cast
        expr = construct(tb);
        if (P4Scope::prop.width_unknown) {
            expr = new IR::Cast(tb, expr);
            P4Scope::prop.width_unknown = false;
        }
        expr = new IR::Cmpl(tb, expr);
    } break;
    case 2: {
        // pick a cast to the type that matches the type
        // new bit type can be random here
        expr = new IR::Cast(tb, expression_bit::construct(tb));
    } break;
    case 3: {
        auto p4_functions = P4Scope::get_decls<IR::Function>();
        auto p4_externs = P4Scope::get_decls<IR::Method>();

        IR::IndexedVector<IR::Declaration> viable_functions;
        for (auto fun : p4_functions) {
            if (fun->type->returnType->to<IR::Type_Bits>()) {
                viable_functions.push_back(fun);
            }
        }
        for (auto fun : p4_externs) {
            if (fun->type->returnType->to<IR::Type_Bits>()) {
                viable_functions.push_back(fun);
            }
        }
        const IR::Type *ret_type;
        expr = expression::pick_function(viable_functions, &ret_type);
        // can not find a suitable function, generate a default value
        if (not expr) {
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

IR::Expression *expression_bit::construct_binary_expr(const IR::Type_Bits *tb) {
    IR::Expression *expr = nullptr;

    if (P4Scope::prop.depth > MAX_DEPTH) {
        return baseType::gen_bit_literal(tb);
    }
    P4Scope::prop.depth++;

    auto pct_sub = PCT.EXPRESSION_BIT_BINARY_SUB;
    auto pct_subsat = PCT.EXPRESSION_BIT_BINARY_SUBSAT;
    // we want to avoid subtraction when we require no negative values
    if (P4Scope::req.not_negative) {
        pct_sub = 0;
        pct_subsat = 0;
    }
    std::vector<int64_t> percent = {PCT.EXPRESSION_BIT_BINARY_MUL,
                                    PCT.EXPRESSION_BIT_BINARY_DIV,
                                    PCT.EXPRESSION_BIT_BINARY_MOD,
                                    PCT.EXPRESSION_BIT_BINARY_ADD,
                                    pct_sub,
                                    PCT.EXPRESSION_BIT_BINARY_ADDSAT,
                                    pct_subsat,
                                    PCT.EXPRESSION_BIT_BINARY_LSHIFT,
                                    PCT.EXPRESSION_BIT_BINARY_RSHIFT,
                                    PCT.EXPRESSION_BIT_BINARY_BAND,
                                    PCT.EXPRESSION_BIT_BINARY_BOR,
                                    PCT.EXPRESSION_BIT_BINARY_BXOR,
                                    PCT.EXPRESSION_BIT_BINARY_CONCAT};

    switch (randind(percent)) {
    case 0: {
        IR::Expression *left = construct(tb);
        IR::Expression *right = construct(tb);
        // pick a multiplication that matches the type
        expr = new IR::Mul(tb, left, right);
    } break;
    case 1: {
        // pick a division that matches the type
        // TODO: Make more sophisticated
        // this requires only compile time known values
        IR::Expression *left = baseType::gen_bit_literal(tb);
        P4Scope::req.not_zero = true;
        IR::Expression *right = baseType::gen_bit_literal(tb);
        P4Scope::req.not_zero = false;
        expr = new IR::Div(tb, left, right);
    } break;
    case 2: {
        // pick a modulo that matches the type
        // TODO: Make more sophisticated
        // this requires only compile time known values
        IR::Expression *left = baseType::gen_bit_literal(tb);
        P4Scope::req.not_zero = true;
        IR::Expression *right = baseType::gen_bit_literal(tb);
        P4Scope::req.not_zero = false;
        expr = new IR::Mod(tb, left, right);
    } break;
    case 3: {
        IR::Expression *left = construct(tb);
        IR::Expression *right = construct(tb);
        // pick an addition that matches the type
        expr = new IR::Add(tb, left, right);
    } break;
    case 4: {
        IR::Expression *left = construct(tb);
        IR::Expression *right = construct(tb);
        // pick a subtraction that matches the type
        expr = new IR::Sub(tb, left, right);
    } break;
    case 5: {
        // width must be known so we cast
        IR::Expression *left = construct(tb);
        if (P4Scope::prop.width_unknown) {
            left = new IR::Cast(tb, left);
            P4Scope::prop.width_unknown = false;
        }
        IR::Expression *right = construct(tb);
        if (P4Scope::prop.width_unknown) {
            right = new IR::Cast(tb, right);
            P4Scope::prop.width_unknown = false;
        }
        // pick a saturating addition that matches the type
        expr = new IR::AddSat(tb, left, right);
    } break;
    case 6: {
        // width must be known so we cast
        IR::Expression *left = construct(tb);
        if (P4Scope::prop.width_unknown) {
            left = new IR::Cast(tb, left);
            P4Scope::prop.width_unknown = false;
        }
        IR::Expression *right = construct(tb);
        if (P4Scope::prop.width_unknown) {
            right = new IR::Cast(tb, right);
            P4Scope::prop.width_unknown = false;
        }
        // pick a saturating addition that matches the type
        expr = new IR::SubSat(tb, left, right);
    } break;
    case 7: {
        // width must be known so we cast
        IR::Expression *left = construct(tb);
        if (P4Scope::prop.width_unknown) {
            left = new IR::Cast(tb, left);
            P4Scope::prop.width_unknown = false;
        }
        // TODO: Make this more sophisticated,
        P4Scope::req.not_negative = true;
        IR::Expression *right = construct(tb);
        P4Scope::req.not_negative = false;
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
        IR::Expression *left = construct(tb);
        if (P4Scope::prop.width_unknown) {
            left = new IR::Cast(tb, left);
            P4Scope::prop.width_unknown = false;
        }

        // TODO: Make this more sophisticated,
        P4Scope::req.not_negative = true;
        IR::Expression *right = construct(tb);
        P4Scope::req.not_negative = false;
        // shifts are limited to 8 bits
        if (tb->width_bits() > 8) {
            right = new IR::Cast(new IR::Type_Bits(8, false), right);
        }
        // pick a right-shift that matches the type
        expr = new IR::Shr(tb, left, right);
    } break;
    case 9: {
        IR::Expression *left = construct(tb);
        IR::Expression *right = construct(tb);
        // pick an binary And that matches the type
        expr = new IR::BAnd(tb, left, right);
    } break;
    case 10: {
        IR::Expression *left = construct(tb);
        IR::Expression *right = construct(tb);
        // pick a binary Or and that matches the type
        expr = new IR::BOr(tb, left, right);
    } break;
    case 11: {
        IR::Expression *left = construct(tb);
        IR::Expression *right = construct(tb);
        // pick an binary Xor that matches the type
        expr = new IR::BXor(tb, left, right);
    } break;
    case 12: {
        // pick an concatenation that matches the type
        size_t type_width = tb->width_bits();
        size_t split = get_rnd_int(1, type_width - 1);
        // TODO: lazy fallback
        if (split >= type_width) {
            return baseType::gen_bit_literal(tb);
        }
        auto tl = new IR::Type_Bits(type_width - split, false);
        auto tr = new IR::Type_Bits(split, false);
        // width must be known so we cast
        // width must be known so we cast
        IR::Expression *left = construct(tl);
        if (P4Scope::prop.width_unknown) {
            left = new IR::Cast(tl, left);
            P4Scope::prop.width_unknown = false;
        }
        IR::Expression *right = construct(tr);
        if (P4Scope::prop.width_unknown) {
            right = new IR::Cast(tr, right);
            P4Scope::prop.width_unknown = false;
        }
        expr = new IR::Concat(tb, left, right);
    } break;
    }
    return expr;
}

IR::Expression *
expression_bit::construct_ternary_expr(const IR::Type_Bits *tb) {
    IR::Expression *expr = nullptr;

    if (P4Scope::prop.depth > MAX_DEPTH) {
        return baseType::gen_bit_literal(tb);
    }
    P4Scope::prop.depth++;
    std::vector<int64_t> percent = {PCT.EXPRESSION_BIT_BINARY_SLICE,
                                    PCT.EXPRESSION_BIT_BINARY_MUX};

    switch (randind(percent)) {
    case 0: {
        // TODO: Refine this...
        // pick a slice that matches the type
        auto type_width = tb->width_bits();
        // TODO this is some arbitrary value...
        auto new_type_size = get_rnd_int(1, 128) + type_width;
        auto slice_type = new IR::Type_Bits(new_type_size, false);
        auto slice_expr = construct(slice_type);
        if (P4Scope::prop.width_unknown) {
            slice_expr = new IR::Cast(slice_type, slice_expr);
            P4Scope::prop.width_unknown = false;
        }
        auto margin = new_type_size - type_width;
        size_t high = get_rnd_int(0, margin - 1) + type_width;
        size_t low = high - type_width + 1;
        expr = new IR::Slice(slice_expr, high, low);
        break;
    }
    case 1: {
        // pick a mux that matches the type
        IR::Expression *cond = expression_boolean::construct();
        IR::Expression *left = construct(tb);
        if (P4Scope::prop.width_unknown) {
            left = new IR::Cast(tb, left);
            P4Scope::prop.width_unknown = false;
        }
        IR::Expression *right = construct(tb);
        if (P4Scope::prop.width_unknown) {
            right = new IR::Cast(tb, right);
            P4Scope::prop.width_unknown = false;
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

IR::Expression *expression_bit::construct(const IR::Type_Bits *tb) {
    IR::Expression *expr = nullptr;
    std::vector<int64_t> percent = {
        PCT.EXPRESSION_BIT_VAR,         PCT.EXPRESSION_BIT_INT_LITERAL,
        PCT.EXPRESSION_BIT_BIT_LITERAL, PCT.EXPRESSION_BIT_UNARY,
        PCT.EXPRESSION_BIT_BINARY,      PCT.EXPRESSION_BIT_TERNARY};

    switch (randind(percent)) {
    case 0: {
        // pick a variable that matches the type
        // do not pick, if the requirement is to be a compile time known value
        // TODO: This is lazy, we can easily check
        if (P4Scope::req.compile_time_known) {
            expr = baseType::gen_bit_literal(tb);
        } else {
            expr = pick_var(tb);
        }
    } break;
    case 1: {
        // pick an int literal, if allowed
        if (P4Scope::req.require_scalar) {
            expr = baseType::gen_bit_literal(tb);
        } else {
            bool has_int = P4Scope::check_lval(new IR::Type_InfInt());

            if (get_rnd_int(0, 1) || not has_int) {
                expr = baseType::gen_int_literal(tb->width_bits());
            } else {
                cstring name = P4Scope::pick_lval(new IR::Type_InfInt());
                expr = new IR::PathExpression(name);
            }
            P4Scope::prop.width_unknown = true;
        }
    } break;
    case 2: {
        // pick a bit literal that matches the type
        expr = baseType::gen_bit_literal(tb);
    } break;
    case 3: {
        // pick a unary expression that matches the type
        expr = construct_unary_expr(tb);
    } break;
    case 4: {
        // pick a binary expression that matches the type
        expr = construct_binary_expr(tb);
    } break;
    case 5: {
        // pick a ternary expression that matches the type
        expr = construct_ternary_expr(tb);
    } break;
    }
    return expr;
}

} // namespace CODEGEN

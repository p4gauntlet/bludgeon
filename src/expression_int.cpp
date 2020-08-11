#include "expression_int.h"

#include "argument.h"
#include "baseType.h"
#include "scope.h"

namespace CODEGEN {

IR::Expression *expression_int::construct_unary_expr() {
    IR::Expression *expr = nullptr;

    if (P4Scope::prop.depth > MAX_DEPTH) {
        return baseType::gen_int_literal();
    }
    IR::Type *tp = new IR::Type_InfInt();
    P4Scope::prop.depth++;

    // we want to avoid negation when we require no negative values
    int64_t neg_pct = PCT.EXPRESSION_INT_UNARY_NEG;
    if (P4Scope::req.not_negative) {
        neg_pct = 0;
    }
    std::vector<int64_t> percent = {neg_pct, PCT.EXPRESSION_INT_UNARY_FUNCTION};

    switch (randind(percent)) {
    case 0: {
        // pick a negation that matches the type
        expr = new IR::Neg(tp, expression_int::construct());
    } break;
    case 1: {
        auto p4_functions = P4Scope::get_decls<IR::Function>();
        auto p4_externs = P4Scope::get_decls<IR::Method>();

        IR::IndexedVector<IR::Declaration> viable_functions;
        for (auto fun : p4_functions) {
            if (fun->type->returnType->to<IR::Type_InfInt>()) {
                viable_functions.push_back(fun);
            }
        }
        for (auto fun : p4_externs) {
            if (fun->type->returnType->to<IR::Type_InfInt>()) {
                viable_functions.push_back(fun);
            }
        }
        const IR::Type *ret_type;
        expr = expression::pick_function(viable_functions, &ret_type);
        // can not find a suitable function, generate a default value
        if (not expr) {
            expr = baseType::gen_int_literal();
            break;
        }
    } break;
    }
    return expr;
}

IR::Expression *expression_int::construct_binary_expr() {
    IR::Expression *expr = nullptr;
    if (P4Scope::prop.depth > MAX_DEPTH) {
        return baseType::gen_int_literal();
    }
    IR::Type *tp = new IR::Type_InfInt();
    P4Scope::prop.depth++;

    auto pct_sub = PCT.EXPRESSION_INT_BINARY_SUB;
    // we want to avoid subtraction when we require no negative values
    if (P4Scope::req.not_negative) {
        pct_sub = 0;
    }
    std::vector<int64_t> percent = {PCT.EXPRESSION_INT_BINARY_MUL,
                                    PCT.EXPRESSION_INT_BINARY_DIV,
                                    PCT.EXPRESSION_INT_BINARY_MOD,
                                    PCT.EXPRESSION_INT_BINARY_ADD,
                                    pct_sub,
                                    PCT.EXPRESSION_INT_BINARY_LSHIFT,
                                    PCT.EXPRESSION_INT_BINARY_RSHIFT,
                                    PCT.EXPRESSION_INT_BINARY_BAND,
                                    PCT.EXPRESSION_INT_BINARY_BOR,
                                    PCT.EXPRESSION_INT_BINARY_BXOR};

    switch (randind(percent)) {
    case 0: {
        IR::Expression *left = construct();
        IR::Expression *right = construct();
        // pick a multiplication that matches the type
        expr = new IR::Mul(tp, left, right);
    } break;
    case 1: {
        // pick a division that matches the type
        // TODO: Make more sophisticated
        P4Scope::req.not_negative = true;
        IR::Expression *left = baseType::gen_int_literal();
        P4Scope::req.not_zero = true;
        IR::Expression *right = baseType::gen_int_literal();
        P4Scope::req.not_zero = false;
        P4Scope::req.not_negative = false;
        expr = new IR::Div(tp, left, right);
    } break;
    case 2: {
        // pick a modulo that matches the type
        // TODO: Make more sophisticated
        P4Scope::req.not_negative = true;
        IR::Expression *left = baseType::gen_int_literal();
        P4Scope::req.not_zero = true;
        IR::Expression *right = baseType::gen_int_literal();
        P4Scope::req.not_zero = false;
        P4Scope::req.not_negative = false;
        expr = new IR::Mod(tp, left, right);
    } break;
    case 3: {
        IR::Expression *left = construct();
        IR::Expression *right = construct();
        // pick an addition that matches the type
        expr = new IR::Add(tp, left, right);
    } break;
    case 4: {
        IR::Expression *left = construct();
        IR::Expression *right = construct();
        // pick a subtraction that matches the type
        expr = new IR::Sub(tp, left, right);
    } break;
    case 5: {
        // width must be known so we cast
        IR::Expression *left = construct();
        // TODO: Make this more sophisticated,
        P4Scope::req.not_negative = true;
        IR::Expression *right = construct();
        // shifts are limited to 8 bits
        right = new IR::Cast(new IR::Type_Bits(8, false), right);
        P4Scope::req.not_negative = false;
        expr = new IR::Shl(tp, left, right);
    } break;
    case 6: {
        // width must be known so we cast
        IR::Expression *left = construct();
        // TODO: Make this more sophisticated,
        P4Scope::req.not_negative = true;
        IR::Expression *right = construct();
        // shifts are limited to 8 bits
        right = new IR::Cast(new IR::Type_Bits(8, false), right);
        P4Scope::req.not_negative = false;
        expr = new IR::Shr(tp, left, right);
    } break;
    case 7: {
        IR::Expression *left = construct();
        IR::Expression *right = construct();
        // pick an binary And that matches the type
        expr = new IR::BAnd(tp, left, right);
    } break;
    case 8: {
        IR::Expression *left = construct();
        IR::Expression *right = construct();
        // pick a binary Or and that matches the type
        expr = new IR::BOr(tp, left, right);
    } break;
    case 9: {
        IR::Expression *left = construct();
        IR::Expression *right = construct();
        // pick an binary Xor that matches the type
        expr = new IR::BXor(tp, left, right);
    } break;
    }
    return expr;
}

IR::Expression *pick_var() {
    IR::Type *tp = new IR::Type_InfInt();
    if (P4Scope::check_lval(tp)) {
        cstring name = P4Scope::pick_lval(tp);
        return new IR::PathExpression(name);
    }

    // fallback, just generate a literal
    return baseType::gen_int_literal();
}

IR::Expression *expression_int::construct() {
    IR::Expression *expr = nullptr;
    std::vector<int64_t> percent = {
        PCT.EXPRESSION_INT_VAR, PCT.EXPRESSION_INT_INT_LITERAL,
        PCT.EXPRESSION_INT_UNARY, PCT.EXPRESSION_INT_BINARY};

    switch (randind(percent)) {
    case 0: {
        expr = pick_var();
    } break;
    case 1: {
        // pick an int literal that matches the type
        expr = baseType::gen_int_literal();
    } break;
    case 2: {
        // pick a unary expression that matches the type
        expr = construct_unary_expr();
    } break;
    case 3: {
        // pick a binary expression that matches the type
        expr = construct_binary_expr();
    } break;
    }
    return expr;
}

} // namespace CODEGEN

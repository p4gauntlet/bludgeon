#include "expression_2.h"

namespace CODEGEN {
IR::Expression *construct_bit_expr(const IR::Type_Bits *tb);
IR::Expression *construct_boolean_expr();
IR::Expression *construct_structlike_expr();

IR::Expression *construct_unary_expr(const IR::Type_Bits *tb) {
    IR::Expression *expr = nullptr;

    switch (rand() % 3) {
        case 0: {
                // pick a negation that matches the type
                expr = new IR::Neg(construct_bit_expr(tb));
            }
            break;
        case 1: {
                // pick a complement that matches the type
                expr = new IR::Cmpl(construct_bit_expr(tb));
            }
            break;
        case 2: {
                // pick a cast to the type that matches the type
                // new bit type can be random here
                expr = new IR::Cast(tb, construct_bit_expr(tb));
            }
            break;
    }
    return expr;
}


IR::Expression *construct_binary_expr(const IR::Type_Bits *tb) {
    IR::Expression *expr  = nullptr;
    IR::Expression *left  = construct_bit_expr(tb);
    IR::Expression *right = construct_bit_expr(tb);

    switch (rand() % 13) {
        case 0: {
                // pick a multiplication that matches the type
                expr = new IR::Mul(left, right);
            }
            break;
        case 1: {
                // pick a division that matches the type
                expr = new IR::Div(left, right);
            }
            break;
        case 2: {
                // pick a modulo that matches the type
                expr = new IR::Mod(left, right);
            }
            break;
        case 3: {
                // pick an addition that matches the type
                expr = new IR::Add(left, right);
            }
            break;
        case 4: {
                // pick a subtraction that matches the type
                expr = new IR::Sub(left, right);
            }
            break;
        case 5: {
                // pick a saturating addition that matches the type
                expr = new IR::AddSat(left, right);
            }
            break;
        case 6: {
                // pick a saturating subtraction that matches the type
                expr = new IR::SubSat(left, right);
            }
            break;
        case 7: {
                // pick a left-shift that matches the type
                expr = new IR::Shl(left, right);
            }
            break;
        case 8: {
                // pick a right-shift that matches the type
                expr = new IR::Shr(left, right);
            }
            break;
        case 9: {
                // pick an binary And that matches the type
                expr = new IR::BAnd(left, right);
            }
            break;
        case 10: {
                // pick a binary Or and that matches the type
                expr = new IR::BOr(left, right);
            }
            break;
        case 11: {
                // pick an binary Xor that matches the type
                expr = new IR::BXor(left, right);
            }
            break;
        case 12: {
                // pick an concatenation that matches the type
                // ideally the concatenation should sum up to the type we want
                expr = new IR::Concat(left, right);
            }
            break;
    }
    return expr;
}


IR::Expression *construct_ternary_expr(const IR::Type_Bits *tb) {
    IR::Expression *expr = nullptr;

    switch (rand() % 1) {
        case 0: {
                // pick a mux that matches the type
                IR::Expression *cond  = construct_boolean_expr();
                IR::Expression *left  = construct_bit_expr(tb);
                IR::Expression *right = construct_bit_expr(tb);
                expr = new IR::Mux(cond, left, right);
            }
            break;
            // case 1:
            //     // pick a slice that matches the type
            //     // expr =
            //     break;
    }
    return expr;
}


IR::Expression *construct_bit_expr(const IR::Type_Bits *tb) {
    IR::Expression *expr = nullptr;

    switch (rand() % 4) {
        case 0: {
                // pick a variable that matches the type
                cstring type_key = tb->toString();
                if (P4Scope::lval_map.find(type_key) !=
                    P4Scope::lval_map.end()) {
                    expr = new IR::PathExpression((P4Scope::pick_lval(tb)));
                } else {
                    // reroll
                    expr = bit_literal::gen_bits(tb);
                }
            }
                break;
        case 1: {
                // pick a literal that matches the type or an int
                //  I think gen literal does not work like this
                // should be different
                expr = bit_literal::gen_literal();
            }
            break;
        case 2: {
                // pick a unary expression that matches the type
                expr = construct_unary_expr(tb);
            }
            break;
        case 3: {
                // pick a binary expression that matches the type
                expr = construct_binary_expr(tb);
            }
            break;
        case 4: {
                // pick a ternary expression that matches the type
                expr = construct_ternary_expr(tb);
            }
            break;
    }
    return expr;
}


IR::Expression *construct_cmp_expr() {
    IR::Expression *expr = nullptr;

    // gen some random type
    // can be either bits, int, bool, or structlike
    switch (rand() % 2) {
        case 0: {
                // pick an equals that matches the type
            }
            break;
        case 1: {
                // pick a not-equals that matches the type
            }
            break;
    }
    return expr;
}


IR::Expression *construct_boolean_expr() {
    IR::Expression *expr = nullptr;
    IR::Expression *left;
    IR::Expression *right;

    switch (rand() % 6) {
        case 0: {
                // pick a boolean variable from available the declarations
            }
            break;
        case 1: {
                // pick a boolean literal
                expr = bool_literal::gen_literal();
            }
            break;
        case 2: {
                // pick a Not expression
                expr = new IR::LNot(construct_boolean_expr());
            }
            break;
        case 3: {
                // pick an And expression
                left  = construct_boolean_expr();
                right = construct_boolean_expr();
                expr  = new IR::LAnd(left, right);
            }
            break;
        case 4: {
                // pick an Or expression
                left  = construct_boolean_expr();
                right = construct_boolean_expr();
                expr  = new IR::LOr(left, right);
            }
            break;
        case 5: {
                // pick a comparison
                expr = construct_cmp_expr();
            }
            break;
    }
    return expr;
}


IR::Expression *construct_structlike_expr() {
    IR::Expression *expr = nullptr;

    return expr;
}


IR::Expression *expression2::gen_expr(const IR::Type *tp) {
    IR::Expression *expr = nullptr;

    // TODO: Add specific restrictions to types later
    if (auto tb = tp->to<IR::Type_Bits>()) {
        expr = construct_bit_expr(tb);
    } else if (tp->is<IR::Type_Boolean>()) {
        expr = construct_boolean_expr();
    } else if (tp->is<IR::Type_StructLike>()) {
        // ultimately we can only return a struct here right?
        expr = construct_structlike_expr();
    } else {
        BUG("Type %s not yet supported", tp->node_type_name());
    }
    return expr;
}


IR::Expression *expression2::gen_input_arg(const IR::Parameter *param) {
    IR::Direction dir = param->direction;

    if (dir == IR::Direction::In) {
        return expression2::gen_expr(param->type);
    } else{
        return new IR::PathExpression(P4Scope::pick_lval(param->type));
    }
}
} // namespace CODEGEN

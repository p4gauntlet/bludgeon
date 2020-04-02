#include "expression.h"

namespace CODEGEN {
IR::Expression *construct_bit_expr(const IR::Type_Bits *tb,
                                   bool                require_width = false);
IR::Expression *construct_boolean_expr();
IR::Expression *construct_structlike_expr();

IR::Expression *construct_unary_expr(const IR::Type_Bits *tb,
                                     bool                require_width =
                                         false) {
    IR::Expression *expr = nullptr;

    switch (rand() % 3) {
        case 0: {
                // pick a negation that matches the type
                expr = new IR::Neg(tb, construct_bit_expr(tb, require_width));
            }
            break;
        case 1: {
                // pick a complement that matches the type
                // width must be known so we cast
                expr = construct_bit_expr(tb, true);
                // for now insert a cast because I do not understand
                // how to resolve types nicely
                expr = new IR::Cmpl(tb, expr);
                expr = new IR::Cast(tb, expr);
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
    IR::Expression *expr = nullptr;
    // todo add a restricted methodcallexpression here
    std::vector<int> percent =
    { 5, 5, 5, 10, 10, 10, 10, 5, 5, 10, 10, 10, 5 };

    switch (randind(percent)) {
        case 0: {
                IR::Expression *left  = construct_bit_expr(tb);
                IR::Expression *right = construct_bit_expr(tb);
                // pick a multiplication that matches the type
                expr = new IR::Mul(tb, left, right);
            }
            break;
        case 1: {
                // pick a division that matches the type
                // TODO: Make more sophisticated
                IR::Expression *left  = bit_literal::gen_bit(tb);
                IR::Expression *right = bit_literal::gen_bit(tb);

                expr = new IR::Div(tb, left, right);
            }
            break;
        case 2: {
                // TODO: Make more sophisticated
                IR::Expression *left  = bit_literal::gen_bit(tb);
                IR::Expression *right = bit_literal::gen_bit(tb);
                expr = new IR::Mod(tb, left, right);
            }
            break;
        case 3: {
                IR::Expression *left  = construct_bit_expr(tb);
                IR::Expression *right = construct_bit_expr(tb);
                // pick an addition that matches the type
                expr = new IR::Add(tb, left, right);
            }
            break;
        case 4: {
                IR::Expression *left  = construct_bit_expr(tb);
                IR::Expression *right = construct_bit_expr(tb);
                // pick a subtraction that matches the type
                expr = new IR::Sub(tb, left, right);
            }
            break;
        case 5: {
                // width must be known so we cast
                IR::Expression *left  = construct_bit_expr(tb, true);
                IR::Expression *right = construct_bit_expr(tb, true);
                // pick a saturating addition that matches the type
                expr = new IR::AddSat(tb, left, right);
            }
            break;
        case 6: {
                // width must be known so we cast
                IR::Expression *left  = construct_bit_expr(tb, true);
                IR::Expression *right = construct_bit_expr(tb, true);
                // pick a saturating addition that matches the type
                expr = new IR::SubSat(tb, left, right);
            }
            break;
        case 7: {
                // width must be known so we cast
                IR::Expression *left  = construct_bit_expr(tb, true);
                IR::Expression *right = construct_bit_expr(tb);
                // TODO: Make this more sophisticated
                // shifts are limited to 8 bits
                right = new IR::Cast(new IR::Type_Bits(8, false), right);
                // pick a left-shift that matches the type
                expr = new IR::Shl(tb, left, right);
            }
            break;
        case 8: {
                // width must be known so we cast
                IR::Expression *left =
                    construct_bit_expr(tb, true);
                IR::Expression *right = construct_bit_expr(tb, true);
                // TODO: Make this more sophisticated,
                // shifts are limited to 8 bits
                right = new IR::Cast(new IR::Type_Bits(8, false), right);
                // pick a right-shift that matches the type
                expr = new IR::Shr(tb, left, right);
            }
            break;
        case 9: {
                IR::Expression *left  = construct_bit_expr(tb);
                IR::Expression *right = construct_bit_expr(tb);
                // pick an binary And that matches the type
                expr = new IR::BAnd(tb, left, right);
            }
            break;
        case 10: {
                IR::Expression *left  = construct_bit_expr(tb);
                IR::Expression *right = construct_bit_expr(tb);
                // pick a binary Or and that matches the type
                expr = new IR::BOr(tb, left, right);
            }
            break;
        case 11: {
                IR::Expression *left  = construct_bit_expr(tb);
                IR::Expression *right = construct_bit_expr(tb);
                // pick an binary Xor that matches the type
                expr = new IR::BXor(tb, left, right);
            }
            break;
        case 12: {
                // pick an concatenation that matches the type
                size_t type_width = tb->width_bits();
                size_t split      = (rand() % type_width) + 1;
                // TODO: lazy fallback
                if (split >= type_width) {
                    return bit_literal::gen_bit(tb);
                }
                auto type_left = new IR::Type_Bits(type_width - split,
                                                   false);
                auto type_right = new IR::Type_Bits(split, false);
                // width must be known so we cast
                IR::Expression *left =
                    new IR::Cast(type_left, construct_bit_expr(tb));
                IR::Expression *right =
                    new IR::Cast(type_right, construct_bit_expr(tb));
                expr = new IR::Concat(tb, left, right);
            }
            break;
    }
    return expr;
}


IR::Expression *construct_ternary_expr(const IR::Type_Bits *tb) {
    IR::Expression *expr     = nullptr;
    std::vector<int> percent = { 50, 50 };

    switch (randind(percent)) {
        case 0: {
                // TODO: Refine this...
                // pick a slice that matches the type
                auto type_width = tb->width_bits();
                // TODO this is some arbitrary value...
                auto new_type_size = rand() % 128 + type_width + 1;
                auto slice_type    = new IR::Type_Bits(new_type_size, false);
                auto slice_expr    = construct_bit_expr(slice_type, true);
                // another cast where there shouldn't be...
                slice_expr = new IR::Cast(slice_type, slice_expr);
                auto margin = new_type_size - type_width;
                size_t high = (rand() % margin) + type_width;
                size_t low  = high - type_width + 1;
                expr = new IR::Slice(slice_expr, high, low);
                break;
            }
        case 1: {
                // pick a mux that matches the type
                IR::Expression *cond = construct_boolean_expr();
                IR::Expression *left = construct_bit_expr(tb);
                //Type Inference for Mux does not quite work so we cast...
                left = new IR::Cast(tb, left);
                IR::Expression *right = construct_bit_expr(tb);
                // insert cast here too....
                right = new IR::Cast(tb, right);
                expr  = new IR::Mux(tb, cond, left, right);
            }
            break;
    }
    return expr;
}


IR::Expression *pick_var(const IR::Type_Bits *tb) {
    cstring type_key     = tb->toString();
    cstring node_name    = tb->node_type_name();
    auto avail_bit_types = P4Scope::lval_map[node_name].size();

    if (avail_bit_types > 0) {
        if (P4Scope::lval_map.count(type_key) != 0) {
            auto candidates = P4Scope::lval_map[node_name][tb->width_bits()];
            size_t idx      = rand() % candidates.size();
            auto lval       = std::begin(candidates);
            // 'advance' the iterator n times
            std::advance(lval, idx);
            return new IR::PathExpression(*lval);
        } else {
            auto bit_types      = P4Scope::lval_map[node_name];
            size_t bit_type_idx = rand() % bit_types.size();
            auto it             = bit_types.begin();
            std::advance(it, bit_type_idx);

            size_t idx = rand() % it->second.size();
            auto lval       = std::begin(it->second);
            // 'advance' the iterator n times
            std::advance(lval, idx);
            return new IR::Cast(tb, new IR::PathExpression(*lval));
        }
    }

    //fallback, just generate a literal
    return bit_literal::gen_bit(tb);
}


IR::Expression *construct_bit_expr(const IR::Type_Bits *tb,
                                   bool                require_width) {
    IR::Expression *expr     = nullptr;
    std::vector<int> percent = { 15, 15, 20, 10, 20, 10 };

    switch (randind(percent)) {
        case 0: {
                // pick a variable that matches the type
                expr = pick_var(tb);
            }
            break;
        case 1: {
                // pick an int literal if allowed
                //  I think gen literal does not work like this
                // should be different
                // this should work
                // but causes a floating point exception sometimes...
                if (require_width) {
                    expr = bit_literal::gen_bit(tb);
                } else {
                    big_int max_size = ((big_int)2) ^ (tb->width_bits() - 1);
                    expr = bit_literal::gen_int(100);
                }
            }
            break;
        case 2: {
                // pick a bit literal that matches the type
                expr = bit_literal::gen_bit(tb);
            }
            break;
        case 3: {
                // pick a unary expression that matches the type
                expr = construct_unary_expr(tb, require_width);
            }
            break;
        case 4: {
                // pick a binary expression that matches the type
                expr = construct_binary_expr(tb);
            }
            break;
        case 5: {
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
    // for now it is just bits
    auto new_type_size    = rand() % 128 + 1;
    auto new_type         = new IR::Type_Bits(new_type_size, false);
    IR::Expression *left  = construct_bit_expr(new_type);
    IR::Expression *right = construct_bit_expr(new_type);

    std::vector<int> percent = { 50, 50 };

    switch (randind(percent)) {
        case 0: {
                expr = new IR::Equ(left, right);
                // pick an equals that matches the type
            }
            break;
        case 1: {
                expr = new IR::Neq(left, right);
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

    std::vector<int> percent = { 0, 40, 40, 5, 5, 10 };

    switch (randind(percent)) {
        case 0: {
                // pick a boolean variable from available the declarations
                expr = bool_literal::gen_literal();
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


IR::Expression *expression::gen_literal(int tp_literal) {
    if (tp_literal != -1) {
        switch (tp_literal) {
            case 0: return bool_literal::gen_literal();

                break;
            case 1: return bit_literal::gen_int();

                break;
            case 2: return new IR::StringLiteral(CODEGEN::randstr(6));

                break;
        }
    }

    return nullptr;
}


IR::Expression *expression::gen_expr(const IR::Type *tp, bool require_width) {
    IR::Expression *expr = nullptr;

    // TODO: Add specific restrictions to types later
    if (auto tb = tp->to<IR::Type_Bits>()) {
        expr = construct_bit_expr(tb, require_width);
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


IR::Expression *expression::gen_input_arg(const IR::Parameter *param) {
    if (param->direction == IR::Direction::In) {
        return expression::gen_expr(param->type);
    } else{
        cstring lval = P4Scope::pick_lval(param->type, true);
        return new IR::PathExpression(lval);
    }
}


bool expression::check_input_arg(const IR::Parameter *param) {
    if (param->direction == IR::Direction::In) {
        return P4Scope::check_lval(param->type, false);
    } else {
        return P4Scope::check_lval(param->type, true);
    }
}
} // namespace CODEGEN

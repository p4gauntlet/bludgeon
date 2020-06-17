#include "baseType.h"
#include "common.h"
#include "scope.h"

namespace CODEGEN {

const int baseType::bit_widths[5];

IR::Type_Base *baseType::pick_rnd_base_type(std::vector<int> &type) {
    if (type.size() == 0) {
        BUG("pick_rnd_base_type: Type list cannot be empty");
    }
    IR::Type_Base *tb = nullptr;
    int t = type.at(get_rnd_int(0, type.size() - 1));
    if (t > 3) {
        BUG("pick_rnd_base_type: Invalid value");
    }
    switch (t) {
    case 0: {
        // bool
        tb = gen_bool_type();
        break;
    }
    case 1: {
        // bit<>
        tb = gen_bit_type(false);
        break;
    }
    case 2: {
        // int<>, this is not supported right now
        tb = gen_bit_type(true);
        break;
    }
    }

    return tb;
}

IR::BoolLiteral *baseType::gen_bool_literal() {
    if (get_rnd_int(0, 1)) {
        return new IR::BoolLiteral(false);
    } else {
        return new IR::BoolLiteral(true);
    }
}

// isSigned, true -> int<>, false -> bit<>
// Tao: we only use false here
IR::Type_Bits *baseType::gen_bit_type(bool isSigned) {
    auto size = get_rnd_int(0, sizeof(bit_widths) / sizeof(int) - 1);

    return new IR::Type_Bits(bit_widths[size], isSigned);
}

IR::Constant *baseType::gen_int_literal(size_t bit_width) {
    big_int min = -(((big_int)1 << bit_width - 1));
    if (P4Scope::req.not_negative) {
        min = 0;
    }
    big_int max = (((big_int)1 << bit_width - 1) - 1);
    big_int value = get_rnd_big_int(min, max);
    while (true) {
        if (P4Scope::req.not_zero && value == 0) {
            value = get_rnd_big_int(min, max);
            // retry until we generate a value that is not zero
            continue;
        }
        break;
    }
    return new IR::Constant(value);
}

IR::Constant *baseType::gen_bit_literal(const IR::Type *tb) {
    big_int max_size = ((big_int)1U << tb->width_bits());

    big_int value;
    if (P4Scope::req.not_zero) {
        value = get_rnd_big_int(1, max_size - 1);
    } else {
        value = get_rnd_big_int(0, max_size - 1);
    }
    return new IR::Constant(tb, value);
}

} // namespace CODEGEN

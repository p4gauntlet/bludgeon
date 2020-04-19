#include "baseType.h"

namespace CODEGEN {

const int baseType::bit_widths[5];

IR::Type_Base *baseType::pick_rnd_base_type(std::vector<int> &type) {
    if (type.size() == 0) {
        BUG("pick_rnd_base_type: Type list cannot be empty");
    }
    IR::Type_Base *tb = nullptr;
    int t = type.at(rand() % type.size());
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
    if (rand() % 2 == 0) {
        return new IR::BoolLiteral(false);
    } else {
        return new IR::BoolLiteral(true);
    }
}

// isSigned, true -> int<>, false -> bit<>
// Tao: we only use false here
IR::Type_Bits *baseType::gen_bit_type(bool isSigned) {
    int size = rand() % (sizeof(bit_widths) / sizeof(int));

    return new IR::Type_Bits(bit_widths[size], isSigned);
}

IR::Constant *baseType::gen_int_literal(big_int max_size, bool no_zero) {
    big_int value;
    while (true) {
        value = rand() % max_size;
        if (no_zero && value == 0) {
            // retry until we generate a value that is not zero
            continue;
        }
        break;
    }
    return new IR::Constant(value);
}

IR::Constant *baseType::gen_bit_literal(const IR::Type *tb, bool no_zero) {
    big_int max_size = ((big_int)1U << tb->width_bits());
    big_int value = rand() % max_size;

    while (true) {
        if (no_zero && value == 0) {
            // retry until we generate a value that is not zero
            value = rand() % max_size;
            continue;
        }
        break;
    }
    return new IR::Constant(tb, value);
}

} // namespace CODEGEN

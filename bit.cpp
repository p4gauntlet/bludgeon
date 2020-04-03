#include "bit.h"


namespace CODEGEN {
constexpr int bit_literal::std_size[5];

// isSigned, true -> int<>, false -> bit<>
// Tao: we only use false here
IR::Type_Bits *bit_literal::gen(bool isSigned) {
    int size = rand() % (sizeof(std_size) / sizeof(int));

    return new IR::Type_Bits(std_size[size], isSigned);
}


IR::Constant *bit_literal::gen_int(big_int max_size) {
    return new IR::Constant(rand() % max_size + 1);
}


IR::Constant *bit_literal::gen_bit(const IR::Type *tb) {
    big_int max_size = ((big_int)1U << tb->width_bits()) - 1;
    big_int random_size = rand() % max_size;

    return new IR::Constant(tb, random_size);
}
}

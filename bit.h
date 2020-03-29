#ifndef _BIT_H_
#define _BIT_H_


#include "ir/ir.h"



namespace CODEGEN {
class bit_literal {
public:


    static constexpr int std_size[5] = { 8, 16, 32, 64, 128 };
    bit_literal() {
    }

    // isSigned, true -> int<>, false -> bit<>
    // Tao: we only use false here
    static IR::Type_Bits *gen(bool isSigned) {
        int size = rand() % (sizeof(std_size) / sizeof(int));

        return new IR::Type_Bits(std_size[size], isSigned);
    }

    static IR::Constant *gen_literal() {
        return new IR::Constant(rand() % 6 + 1);
    }

    static IR::Constant *gen_bits(const IR::Type *tb) {
        return new IR::Constant(tb, rand() % 6 + 1);
    }
};
}



#endif

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
    static IR::Type_Bits *gen(bool isSigned);
    static IR::Constant *gen_int(big_int max_size=INT_MAX);
    static IR::Constant *gen_bit(const IR::Type *tb);
};
}



#endif

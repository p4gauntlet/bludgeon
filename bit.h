#ifndef _BIT_H_
#define _BIT_H_


#include "ir/ir.h"




namespace CODEGEN {
class bit_literal {
public:


	static constexpr int std_size[8] = {1, 2, 4, 8, 16, 32, 64, 128};
	bit_literal() {
	}

	// isSigned, true -> int<>, false -> bit<>
	static IR::Type_Bits* gen(bool isSigned) {
		int size = rand()%(sizeof(std_size)/sizeof(int));
		return new IR::Type_Bits(std_size[size], isSigned);
	}

	static IR::Constant* gen_literal() {
		return new IR::Constant(rand()%10+1);
	}
};
}



#endif

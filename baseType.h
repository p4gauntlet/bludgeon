#ifndef _BASETYPE_H_
#define _BASETYPE_H_

#include "ir/ir.h"

#include "bit.h"
#include "bool.h"
#include "int.h"


namespace CODEGEN {


class baseType {
public:
	const char* types[10] = {
		"BOOL",
		"ERROR", // not 
		"INT",
		"BIT",
		"BIT <INTEGER>",
		"INT <INTEGER>",
		"VARBIT <INTEGER>", // not
		"BIT < (expression) >", // not
		"INT < (expression) >", // not 
		"VARBIT < (expression) >" // not
	};

	bool if_random;
	std::vector<int> type;


	baseType(bool if_rand, std::vector<int> &type) : 
		if_random(if_rand), type(type) {
	}

	IR::Type* gen() {
		int t;
		if (if_random == false) {
			t = type.at(rand()%type.size());
		}
		else {
			t = rand()%4;
		}
		switch (t) {
			case 0: return bool_literal::gen(); // bool
			case 1: return bit_literal::gen(false); // bit<>
            case 2: return bit_literal::gen(true); // int<>
			case 3: return int_literal::gen(); // int
		}

		return nullptr;
	}
};


} // namespace CODEGEN




#endif

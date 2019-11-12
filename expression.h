#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "ir/ir.h"


#include "bool.h"
#include "bit.h"

#include "codegen.h"

namespace CODEGEN {


class expression {
public:
	const char* types[0] = {
	};

	int is_literal = -1; // 0 -> bool 1 -> int 2 -> string
	expression (int is_literal) : is_literal(is_literal) {
	}

	IR::Expression* gen() {
		if (is_literal != -1) {
			switch (is_literal) {
				case 0: return bool_literal::gen_literal(); break;
				case 1: return bit_literal::gen_literal(); break;
				case 2: return new IR::StringLiteral(CODEGEN::randstr(6)); break;
			}
		}

		return nullptr;
	}

};


} // namespace CODEGEN




#endif

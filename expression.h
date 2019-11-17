#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "ir/ir.h"


#include "bool.h"
#include "bit.h"

#include "codegen.h"
#include "scope.h"

namespace CODEGEN {


class expression {
public:
	const char* types[0] = {
	};

	expression (){
    }

	IR::Expression* gen_literal(int tp_literal) {
		if (tp_literal != -1) {
			switch (tp_literal) {
				case 0: return bool_literal::gen_literal(); break;
				case 1: return bit_literal::gen_literal(); break;
				case 2: return new IR::StringLiteral(CODEGEN::randstr(6)); break;
			}
		}

		return nullptr;
	}

    // maybe we can operate on IR::Declaration
    // expression[ expression ] ArrayIndex
    // expression[ expression : expression ] Slice
    // { expressionList } --> can be assigned to tuple, struct, header
    // { kvList }
    // ( expression )
    // !, ~, -, +

    static IR::Expression* gen_cond();
};


} // namespace CODEGEN




#endif

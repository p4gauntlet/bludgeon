#ifndef _SWITCHSTATEMENT_H_
#define _SWITCHSTATEMENT_H_

#include "scope.h"
#include "common.h"
#include "expression.h"

#include "blockStatement.h"
#include "ir/ir.h"

namespace CODEGEN {


class switchStatement {
public:
	const char* types[0] = {
	};

    IR::Expression *expr = nullptr;

	switchStatement() {
	}
    ~switchStatement() {
        if (expr != nullptr) {
            delete expr;
        }
    }

    IR::SwitchStatement *gen();
};


} // namespace CODEGEN




#endif

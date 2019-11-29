#ifndef _KEYELEMENT_H_
#define _KEYELEMENT_H_

#include "ir/ir.h"

#include "annotations.h"
#include "scope.h"
#include "expression.h"
#include "codegen.h"

namespace CODEGEN {


class keyElement {
public:
	const char* types[0] = {
	};

	IR::Annotations* annotations;
	IR::Expression* expr;
	IR::PathExpression* match_kind;


	keyElement() {
		// Tao: let match_kind always be exact right now
		IR::ID name;
		name.name = "exact";
		match_kind = new IR::PathExpression(new IR::Path(name));
	}
	~keyElement() {
		delete annotations;
		delete expr;
		delete match_kind;
	}

	IR::KeyElement *gen() {
		IR::KeyElement* ke = nullptr;
		auto annotat = new Annotations();
		annotations = annotat->gen();
		// TODO: how to generate meaningful expr
		IR::Expression* expr = nullptr;
		const IR::Type *tp;
		cstring str_type;
		int num_trials = 100;
		while(num_trials--) {
			expr = expression::get_operand(rand()%2, &tp, str_type, false);
			if (expr != nullptr) {
				break;
			}
		}

		if (expr != nullptr) {
			ke = new IR::KeyElement(annotations, expr, match_kind);
		}

		return ke;
	}
};


} // namespace CODEGEN




#endif

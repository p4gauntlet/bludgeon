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

	IR::KeyElement *gen() {
		auto annotat = new Annotations();
		annotations = annotat->gen();
		// TODO: how to generate meaningful expr
		auto expr_gen = new expression();
		expr = expr_gen->gen_literal(1);

		// return
		return new IR::KeyElement(annotations, expr, match_kind);
	}
};


} // namespace CODEGEN




#endif

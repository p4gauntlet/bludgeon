#ifndef _KEYELEMENT_H_
#define _KEYELEMENT_H_

#include "ir/ir.h"

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
		// TODO: expr, possiblely
	}
};


} // namespace CODEGEN




#endif

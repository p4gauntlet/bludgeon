#ifndef _SPECIFIEDIDENTIFIER_H_
#define _SPECIFIEDIDENTIFIER_H_

#include "ir/ir.h"

#include "codegen.h"
#include "expression.h"

namespace CODEGEN {


class specifiedIdentifier {
public:
	const char* types[0] = {
	};

	std::set<cstring> members_name;

	specifiedIdentifier() {
	}

	// TODO: Tao: better expr generation
	IR::IndexedVector< IR::SerEnumMember > gen(size_t len) {
		IR::IndexedVector< IR::SerEnumMember > members;


		for (size_t i=0; i<len; i++) {
			IR::ID* name = new IR::ID(CODEGEN::randstr(2));
			auto expr = new expression(1); // for bit literal
			IR::Expression* ex = expr->gen();

			if (members_name.find(name->name) != members_name.end()) {
				delete name;
				delete expr;
				delete ex;
				continue;
			}

			auto member = new IR::SerEnumMember(*name, ex);

			members.push_back(member);
		}

		return members;
	}
};


} // namespace CODEGEN




#endif

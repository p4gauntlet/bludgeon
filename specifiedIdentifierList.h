#ifndef _SPECIFIEDIDENTIFIERLIST_H_
#define _SPECIFIEDIDENTIFIERLIST_H_

#include "ir/ir.h"

#include "codegen.h"
#include "expression.h"

namespace CODEGEN {


class specifiedIdentifierList {
public:
	const char* types[1] = {
		"specifiedIdentifier"
	};

	std::set<cstring> members_name;

	specifiedIdentifierList() {
	}

	// TODO: Tao: better expr generation
	IR::IndexedVector< IR::SerEnumMember > gen(size_t len) {
		IR::IndexedVector< IR::SerEnumMember > members;


		for (size_t i=0; i<len; i++) {
			IR::ID* name = new IR::ID(CODEGEN::randstr(2));
			auto expr = new expression(); // for bit literal
			IR::Expression* ex = expr->gen_literal(1);

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
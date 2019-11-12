#ifndef _TABLEDECLARATION_H_
#define _TABLEDECLARATION_H_

#include "ir/ir.h"

#include "codegen.h"
#include "keyElement.h"

namespace CODEGEN {


class tableDeclaration {
public:
	const char* types[0] = {
	};

	IR::ID* name;
	IR::TableProperties* tb_properties;


	tableDeclaration() {
		name = new IR::ID(CODEGEN::randstr(6));
	}

	IR::P4Table* gen() {
	}
};


} // namespace CODEGEN




#endif

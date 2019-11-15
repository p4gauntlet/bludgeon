#ifndef _TABLEDECLARATION_H_
#define _TABLEDECLARATION_H_

#include "ir/ir.h"

#include "codegen.h"
#include "tablePropertyList.h"

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
        auto tab_property_list_gen = new tablePropertyList();


        return new IR::P4Table(*name, tab_property_list_gen->gen());
	}
};


} // namespace CODEGEN




#endif

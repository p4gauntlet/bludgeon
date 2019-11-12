#ifndef _TABLEPROPERTY_H_
#define _TABLEPROPERTY_H_

#include "ir/ir.h"


#include "codegen.h"

#include "keyElementList.h"


namespace CODEGEN {


class tableProperty {
public:
	const char* types[0] = {
	};


	tableProperty() {
	}

	IR::Property* gen_keys() {
		IR::ID* name = new IR::ID("key");
		auto key_gen = new keyElementList();
		auto keys = key_gen->gen(4);

		// isConstant ???
		return new IR::Property(*name, keys, false);
	}
};


} // namespace CODEGEN




#endif

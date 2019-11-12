#ifndef _TABLEPROPERTY_H_
#define _TABLEPROPERTY_H_

#include "ir/ir.h"


#include "codegen.h"

#include "keyElementList.h"
#include "actionList.h"


namespace CODEGEN {


class tableProperty {
public:
	const char* types[0] = {
	};


	tableProperty() {
	}

	// Tao: note that we always tag the property as not const
	IR::Property* gen_keys() {
		IR::ID* name = new IR::ID("key");
		auto key_gen = new keyElementList();
		auto keys = key_gen->gen(4);

		// isConstant --> false
		return new IR::Property(*name, keys, false);
	}

	IR::Property* gen_act_lists() {
		return nullptr;
	}

};


} // namespace CODEGEN




#endif

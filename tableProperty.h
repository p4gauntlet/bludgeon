#ifndef _TABLEPROPERTY_H_
#define _TABLEPROPERTY_H_

#include "ir/ir.h"


#include "codegen.h"

namespace CODEGEN {


class tableProperty {
public:
	const char* types[0] = {
	};

	IR::ID* name;
	IR::PropertyValue* val;

	tableProperty() {
		name = new IR::ID(CODEGEN::randstr(4));
	}

};


} // namespace CODEGEN




#endif

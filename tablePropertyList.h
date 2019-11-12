#ifndef _TABLEPROPERTYLIST_H_
#define _TABLEPROPERTYLIST_H_

#include "ir/ir.h"

namespace CODEGEN {


class tablePropertyList {
public:
	const char* types[1] = {
		"tableProperty"
	};


	// Keys
	// Actions
	// 


	IR::IndexedVector< IR::Property > tb_properties;

	tablePropertyList() {
	}

};


} // namespace CODEGEN




#endif

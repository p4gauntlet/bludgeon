#ifndef _TABLEPROPERTYLIST_H_
#define _TABLEPROPERTYLIST_H_

#include "ir/ir.h"


#include "tableProperty.h"

namespace CODEGEN {


class tablePropertyList {
public:
	const char* types[1] = {
		"tableProperty"
	};


	// Keys
	// Actions
	// 



	tablePropertyList() {
	}

	IR::IndexedVector< IR::Property > gen() {
		IR::IndexedVector< IR::Property > tab_properties;



		return tab_properties;
	}
};


} // namespace CODEGEN




#endif

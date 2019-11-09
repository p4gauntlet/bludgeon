#ifndef _SWITCHLABEL_H_
#define _SWITCHLABEL_H_

#include "ir/ir.h"

namespace CODEGEN {


class switchLabel {
public:
	const char* types[2] = {
		"name",
		"DEFAULT"
	};

	switchLabel() {
	}

};


} // namespace CODEGEN




#endif
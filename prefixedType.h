#ifndef _PREFIXEDTYPE_H_
#define _PREFIXEDTYPE_H_

#include <vector>

#include "ir/ir.h"

#include "common.h"
#include "scope.h"

namespace CODEGEN {


class prefixedType {
public:
	const char* types[2] = {
		"TYPE_IDENTIFIER",
		". TYPE_IDENTIFIER" // not supported
	};

	cstring for_type;
	prefixedType(cstring for_type) : for_type(for_type) {
	}


	IR::Type* gen() {
		// it is a Type Name
		std::vector<cstring> l_types;
		P4Scope::get_all_type_names(for_type, l_types);

		if (l_types.size() == 0) {
			return nullptr;
		}

		IR::ID* name;
		name = new IR::ID(l_types.at(rand()%l_types.size()));


		// we do not want it to have a stack type already
        if (P4Scope::check_type_name(name->name)) {
            return nullptr;
        }
       

		IR::Path* path = new IR::Path(*name);
		return new IR::Type_Name(path);
	}
};


} // namespace CODEGEN




#endif

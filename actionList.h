#ifndef _ACTIONLIST_H_
#define _ACTIONLIST_H_

#include "ir/ir.h"

#include "scope.h"

namespace CODEGEN {

class actionList {
public:
	const char* types[0] = {
	};

	actionList() {
	}
    
    std::set<cstring> act_names;

	IR::ActionList* gen(size_t len) {
		IR::IndexedVector< IR::ActionListElement > act_list;
        std::vector<cstring> nodir_p4act_names = P4Scope::get_name_nodir_p4acts();

        for (size_t i=0; i<len; i++) {
            if (nodir_p4act_names.size() == 0) {
                continue;
            }
            cstring act_name = nodir_p4act_names.at(rand() % nodir_p4act_names.size());

            if (act_names.find(act_name) != act_names.end()) {
                continue;
            }

            act_names.insert(act_name);

            IR::ID* name = new IR::ID(act_name);
            IR::Expression* path_expr = new IR::PathExpression(new IR::Path(*name));
            IR::ActionListElement* actlist_ele = new IR::ActionListElement(path_expr);

            act_list.push_back(actlist_ele);
        }
		return new IR::ActionList(act_list);
	}

};

} // namespace CODEGEN




#endif

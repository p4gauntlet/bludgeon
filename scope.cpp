#include "scope.h"

namespace CODEGEN {

std::vector< IR::Vector<IR::Node>* > P4Scope::scope;
std::set<cstring> P4Scope::used_names;

void P4Scope::add_to_scope(IR::Node* n) {
	auto l_scope = P4Scope::scope.back();
	l_scope->push_back(n);
}

void P4Scope::get_all_type_names(cstring filter, std::vector<cstring> &type_names) {
	for (auto i = scope.begin(); i<scope.end(); i++) {
		for (size_t j=0; j< (*i)->size(); j++) {
			auto obj = (*i)->at(j);

			if (filter == HEADER) {
				if (obj->is<IR::Type_Declaration>() && 
						!obj->is<IR::Type_Header>() &&
						!obj->is<IR::Type_HeaderUnion>()) {
					auto tmp_obj = obj->to<IR::Type_Declaration>();
					type_names.push_back(tmp_obj->name.name);
				}
			}
			else if (filter == HEADER_UNION) { // we only want header def
				if (obj->is<IR::Type_Header>()) {
					auto tmp_obj = obj->to<IR::Type_Declaration>();
					type_names.push_back(tmp_obj->name.name);
				}
			}
			else {
				if (obj->is<IR::Type_Declaration>()) {
					auto tmp_obj = obj->to<IR::Type_Declaration>();
					type_names.push_back(tmp_obj->name.name);
				}
			}
		}
	}
}

int P4Scope::get_num_type_header() {
	int ret = 0;
	for (auto i = scope.begin(); i<scope.end(); i++) {
		for (size_t j=0; j< (*i)->size(); j++) {
			auto obj = (*i)->at(j);
			if (obj->is<IR::Type_Header>()) {
				ret = ret+1;
			}
		}
	}
	return ret;
}

void P4Scope::print_scope() {
	for (auto i = scope.begin(); i<scope.end(); i++) {
		for (size_t j=0; j< (*i)->size(); j++) {
			std::cout << (*i)->at(j) << std::endl;
		}
	}
}


} // namespace CODEGE

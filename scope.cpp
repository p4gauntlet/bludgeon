#include "scope.h"

namespace CODEGEN {

std::vector< IR::Vector<IR::Node>* > P4Scope::scope;
std::set<cstring> P4Scope::used_names;
std::map<const cstring, const IR::Type*> P4Scope::name_2_type_param;
std::map<const cstring, const IR::Type*> P4Scope::name_2_type_vars;
std::map<const cstring, const IR::Type*> P4Scope::name_2_type_const;
std::set<const cstring> P4Scope::types_w_stack;

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
            else if (filter==STRUCT || filter==STRUCT_HEADERS) {
				if (!obj->is<IR::Type_Struct>() &&
                        obj->is<IR::Type_Declaration>()) {
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

std::vector<cstring> P4Scope::get_name_nodir_p4acts() {
	std::vector<cstring> ret;

	std::vector<const IR::P4Action*> nodir_p4acts;
	nodir_p4acts = get_p4actions_nodir();

	for (auto i=nodir_p4acts.begin(); i<nodir_p4acts.end(); i++) {
		ret.push_back((*i)->name.name);
	}

	return ret;
}

std::vector<const IR::P4Action*> P4Scope::get_p4actions_nodir() {
	std::vector<const IR::P4Action*> ret;
	auto p4actions_all = P4Scope::get_p4actions();

	for (auto i=p4actions_all.begin(); i<p4actions_all.end(); i++) {
		const IR::P4Action* p4act = *i;
		auto act_params = p4act->parameters->parameters;
		bool is_p4c_nodir = true;

		for(size_t ind=0; ind<act_params.size(); ind++) {
			auto param = act_params.at(ind);
			if (param->direction != IR::Direction::None) {
				is_p4c_nodir = false;
				break;
			}
		}

		if (is_p4c_nodir == true) {
			ret.push_back(p4act);
		}
	}

	return ret;
}

std::vector<const IR::P4Action*> P4Scope::get_p4actions() {
	std::vector<const IR::P4Action*> ret;
	for (auto i = scope.begin(); i<scope.end(); i++) {
		for (size_t j=0; j< (*i)->size(); j++) {
			auto obj = (*i)->at(j);

			if (obj->is<IR::P4Action>()) {
				const IR::P4Action* tmp_obj = obj->to<IR::P4Action>();
				ret.push_back(tmp_obj);
			}
		}
	}

	return ret;
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

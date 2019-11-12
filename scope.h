#ifndef _SCOPE_H_
#define _SCOPE_H_


#include <vector>
#include "ir/ir.h"


#include "common.h"


namespace CODEGEN {

class P4Scope {
public:

	static std::vector< IR::Vector<IR::Node>* > scope;
	static std::set<cstring> used_names;


	P4Scope() {
	}

	static void start_local_scope() {
		IR::Vector<IR::Node>* local_scope = new IR::Vector<IR::Node>();
		scope.push_back(local_scope);
	}

	static void end_local_scope() {
		auto local_scope = scope.back();
		delete local_scope;
		scope.pop_back();
	}

	static void add_to_scope(IR::Node *n);
	static void get_all_type_names(cstring filter, std::vector<cstring> &type_names);
	static int get_num_type_header();
	static std::vector<const IR::P4Action*> get_p4actions();
	static void print_scope();
};



} // namespace CODEGEN





#endif // _SCOPE_H_

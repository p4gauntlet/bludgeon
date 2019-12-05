#ifndef _SCOPE_H_
#define _SCOPE_H_


#include <vector>
#include <map>
#include <string>
#include "ir/ir.h"


#include "common.h"

namespace CODEGEN {

class P4Scope {
public:

	static std::vector< IR::Vector<IR::Node>* > scope;
	static std::set<cstring> used_names;
    static std::map<cstring, const IR::Type*> name_2_type_param; // param
    static std::map<cstring, const IR::Type*> name_2_type_vars; // variable
    static std::map<cstring, const IR::Type*> name_2_type_const;
	static std::map<cstring, const IR::Type*> name_2_type_stack; // contains stack
    static std::set<cstring> types_w_stack; // which type has a field whose type is stack
	static const IR::Type* ret_type;
	static std::vector<IR::P4Control*> p4_ctrls;
	static std::map<cstring, IR::P4Control*> decl_ins_ctrls;
	static std::map<cstring, IR::P4Action*> decl_actions;

	P4Scope() {
	}

	static void start_local_scope() {
		IR::Vector<IR::Node>* local_scope = new IR::Vector<IR::Node>();
		scope.push_back(local_scope);
	}

	static void end_local_scope();

    static void add_name_2_type_p(cstring name, const IR::Type* type) {
        if (name_2_type_param.find(name) != name_2_type_param.end()) {
            BUG("param wrong name, must be different");
        }
        else {
            name_2_type_param.emplace(name, type);
        }
    }

    static void add_name_2_type_v(cstring name, const IR::Type* type) {
        if (name_2_type_vars.find(name) != name_2_type_vars.end()) {
            BUG("variable wrong name, must be different");
        }
        else {
            name_2_type_vars.emplace(name, type);
        }
    }

    static void add_name_2_type_c(cstring name, const IR::Type* type) {
        if (name_2_type_const.find(name) != name_2_type_const.end()) {
            BUG("const wrong name, must be different");
        }
        else {
            name_2_type_const.emplace(name, type);
        }
    }

    static void add_name_2_type_stack(cstring name, const IR::Type* type) {
        if (name_2_type_stack.find(name) != name_2_type_stack.end()) {
            BUG("const wrong name, must be different");
        }
        else {
            name_2_type_stack.emplace(name, type);
        }
    }



    static bool check_type_name(const cstring &name) {
        return types_w_stack.find(name) != types_w_stack.end();
    }
    static void insert_type_name(const cstring &name) {
        types_w_stack.insert(name);
    }

	static void add_to_scope(IR::Node *n);


	static void get_all_type_names(cstring filter, std::vector<cstring> &type_names);
	static int get_num_type_header();
	static IR::Type* get_type_by_name(cstring name);

	// get all the names of all p4actions w/o dir params
	static std::vector<cstring> get_name_nodir_p4acts();
	// get all p4actions w/o dir params
	static std::vector<const IR::P4Action*> get_p4actions_nodir();

	static std::map<cstring, std::vector<const IR::Type*>> get_action_def();
	// template to get all declarations
	template <typename T>
	static std::vector<const T*> get_decls();



	static void pr_test();
	static void print_scope();
};



} // namespace CODEGEN





#endif // _SCOPE_H_

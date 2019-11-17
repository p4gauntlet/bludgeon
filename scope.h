#ifndef _SCOPE_H_
#define _SCOPE_H_


#include <vector>
#include <map>
#include <string>
#include "ir/ir.h"


#include "common.h"

#define DY_CAST_FUNC function<void*(void*)>

namespace CODEGEN {

class P4Scope {
public:

	static std::vector< IR::Vector<IR::Node>* > scope;
	static std::set<cstring> used_names;
    static std::map<const cstring, const IR::Type*> name_2_type_param;
    static std::map<const cstring, const IR::Type*> name_2_type_vars;
    static std::map<const cstring, const IR::Type*> name_2_type_const;
    static std::set<const cstring> types_w_stack;

	P4Scope() {
	}

	static void start_local_scope() {
		IR::Vector<IR::Node>* local_scope = new IR::Vector<IR::Node>();
		scope.push_back(local_scope);
	}

	static void end_local_scope() {
        IR::Vector<IR::Node>* local_scope = scope.back();

        for (size_t i=0; i<local_scope->size(); i++) {
            auto node = local_scope->at(i);
            if (node->is<IR::Declaration>()) {
                auto decl = node->to<IR::Declaration>();
                name_2_type_param.erase(decl->name.name);
                name_2_type_vars.erase(decl->name.name);
                name_2_type_const.erase(decl->name.name);
            }
        }

		delete local_scope;
		scope.pop_back();
	}

    static void add_name_2_type_p(const cstring &name, const IR::Type* type) {
        std::cout << "insert to param\n";
        std::cout << name << std::endl;
        if (name_2_type_param.find(name) != name_2_type_param.end()) {
            BUG("wrong name, must be different");
        }
        else {
            name_2_type_param.emplace(name, type);
        }
    }

    static void add_name_2_type_v(const cstring &name, const IR::Type* type) {
        if (name_2_type_vars.find(name) != name_2_type_vars.end()) {
            BUG("wrong name, must be different");
        }
        else {
            name_2_type_vars.emplace(name, type);
        }
    }

    static void add_name_2_type_c(const cstring &name, const IR::Type* type) {
        if (name_2_type_const.find(name) != name_2_type_const.end()) {
            BUG("wrong name, must be different");
        }
        else {
            name_2_type_const.emplace(name, type);
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
    static void get_all_decl_names(std::vector<cstring> &decl_names);
	static int get_num_type_header();
	static std::vector<cstring> get_name_nodir_p4acts();
	static std::vector<const IR::P4Action*> get_p4actions_nodir();
	static std::vector<const IR::P4Action*> get_p4actions();
	static void print_scope();
};



} // namespace CODEGEN





#endif // _SCOPE_H_

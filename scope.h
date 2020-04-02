#ifndef _SCOPE_H_
#define _SCOPE_H_


#define SCOPE_PROGRAM    0


#include <vector>
#include <map>
#include <string>
#include "ir/ir.h"


#include "common.h"

namespace CODEGEN {
class P4Scope {
public:

    static std::vector<IR::Vector<IR::Node> *> scope;
    static std::set<cstring> used_names;
    static std::map<cstring, std::map<int, std::set<cstring> > > lval_map;
    // a subset of the lval map that includes rw values
    static std::map<cstring, std::map<int, std::set<cstring> > > lval_map_rw;



    // which type has a field whose type is stack
    static std::set<cstring> types_w_stack;
    static const IR::Type *ret_type;
    static std::vector<IR::P4Control *> p4_ctrls;
    static std::set<const IR::P4Table *> callable_tables;
    static std::map<cstring, IR::P4Control *> decl_ins_ctrls;
    static const IR::Type_Struct *sys_hdr;

    static std::set<cstring> not_initialized_structs;

    // Tao: for refactoring stuff
    static int scope_indicator;                                          // used for indicate where we are
    static std::map<cstring, const IR::Type_StructLike *> compound_type; // name for quick search

    P4Scope() {
    }

    ~P4Scope() {
    }

    static void start_local_scope() {
        IR::Vector<IR::Node> *local_scope = new IR::Vector<IR::Node>();

        scope.push_back(local_scope);
    }

    static void end_local_scope();

    static void add_lval(const IR::Type *tp, cstring name,
                         bool read_only = false);
    static bool check_lval(const IR::Type *tp,
                           bool           must_write = false);
    static cstring pick_lval(const IR::Type *tp,
                             bool           must_write = false);
    static void delete_lval(const IR::Type *tp, cstring name);

    static IR::Type_Bits *pick_declared_bit_type(bool must_write = false);

    IR::Expression *pick_bit_field(const IR::Type_Bits *tp, bool is_lval);



    static bool check_type_name(const cstring& name) {
        return types_w_stack.find(name) != types_w_stack.end();
    }

    static void insert_type_name(const cstring& name) {
        types_w_stack.insert(name);
    }

    static void add_to_scope(const IR::Node *n);


    static void get_all_type_names(cstring               filter,
                                   std::vector<cstring>& type_names);
    static int get_num_type_header();
    static IR::Type *get_type_by_name(cstring name);

    // get all the names of all p4actions w/o dir params
    static std::vector<cstring> get_name_nodir_p4acts();

    // get all p4actions w/o dir params
    static std::vector<const IR::P4Action *> get_p4actions_nodir();

    static std::map<cstring, std::vector<const IR::Type *> > get_action_def();
    static std::vector<const IR::Function *> get_func_decls();
    static std::vector<const IR::P4Table *> get_tab_decls();
    static std::vector<const IR::P4Action *> get_action_decls();
    static std::set<const IR::P4Table *> *get_callable_tables();
    static std::set<cstring> get_candidate_lvals(const IR::Type *tp,
                                                 bool           must_write = true);

    // template to get all declarations
    template<typename T>
    static std::vector<const T *> get_decls();


    static void pr_test();
    static void print_scope();
};
} // namespace CODEGEN



#endif // _SCOPE_H_

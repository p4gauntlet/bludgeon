#ifndef _SCOPE_H_
#define _SCOPE_H_

#include <map>
#include <string>
#include <vector>

#include "common.h"
#include "ir/ir.h"

namespace CODEGEN {
class P4Scope {
  public:
    static std::vector<IR::Vector<IR::Node> *> scope;
    static std::set<cstring> used_names;
    // a map of usable lvalues
    static std::map<cstring, std::map<int, std::set<cstring>>> lval_map;
    // a subset of the lval map that includes rw values
    static std::map<cstring, std::map<int, std::set<cstring>>> lval_map_rw;

    // which type has a field whose type is stack
    static std::set<cstring> types_w_stack;
    static const IR::Type *ret_type;
    static std::vector<IR::P4Control *> p4_ctrls;
    static std::set<const IR::P4Table *> callable_tables;
    static const IR::Type_Struct *sys_hdr;

    static std::set<cstring> not_initialized_structs;

    P4Scope() {}

    ~P4Scope() {}

    static void add_to_scope(const IR::Node *n);
    static void start_local_scope();
    static void end_local_scope();

    static void add_lval(const IR::Type *tp, cstring name,
                         bool read_only = false);
    static bool check_lval(const IR::Type *tp, bool must_write = false);
    static cstring pick_lval(const IR::Type *tp, bool must_write = false);
    static void delete_lval(const IR::Type *tp, cstring name);

    static IR::Type_Bits *pick_declared_bit_type(bool must_write = false);

    IR::Expression *pick_bit_field(const IR::Type_Bits *tp, bool is_lval);

    static bool check_type_name(const cstring &name) {
        return types_w_stack.find(name) != types_w_stack.end();
    }

    static void insert_type_name(const cstring &name) {
        types_w_stack.insert(name);
    }

    static void get_all_type_names(cstring filter,
                                   std::vector<cstring> &type_names);
    static const IR::Type_Declaration *get_type_by_name(cstring name);

    // template to get all declarations
    // C++ is so shit... templates must be inlined to be generally usable.
    template <typename T> static inline std::vector<const T *> get_decls() {
        std::vector<const T *> ret;

        for (auto sub_scope : scope) {
            for (auto node : *sub_scope) {
                if (const T *tmp_obj = node->to<T>()) {
                    ret.push_back(tmp_obj);
                }
            }
        }
        return ret;
    }

    static std::map<cstring, std::vector<const IR::Type *>> get_action_def();
    static std::set<const IR::P4Table *> *get_callable_tables();
    static std::set<cstring> get_candidate_lvals(const IR::Type *tp,
                                                 bool must_write = true);
};
} // namespace CODEGEN

#endif // _SCOPE_H_

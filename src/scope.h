#ifndef _SCOPE_H_
#define _SCOPE_H_

#include <map>
#include <string>
#include <vector>

#include "common.h"
#include "ir/ir.h"

namespace CODEGEN {

struct Requirements {
    bool require_scalar;
    bool compile_time_known;
    bool no_methodcalls;
    bool not_zero;
    bool not_negative;
    bool byte_align_headers;
    Requirements()
        : require_scalar(false),
          compile_time_known(false), no_methodcalls{false}, not_zero(false),
          not_negative(false), byte_align_headers(false){};
};

struct Properties {
    bool width_unknown;
    bool has_methodcall;
    size_t depth = 0;
    // this means we are in a block that returns
    // we need to return an expression with the specified type
    const IR::Type *ret_type = nullptr;
    Properties() : width_unknown(false), has_methodcall{false} {}
};

class P4Scope {
  public:
    // the list of subscopes
    static std::vector<IR::Vector<IR::Node> *> scope;
    // avoid duplicates
    static std::set<cstring> used_names;
    // a map of usable lvalues
    static std::map<cstring, std::map<int, std::set<cstring>>> lval_map;
    // a subset of the lval map that includes rw values
    static std::map<cstring, std::map<int, std::set<cstring>>> lval_map_rw;

    // TODO: Maybe we can just remove tables from the declarations list?
    // this is back end specific
    static std::set<const IR::P4Table *> callable_tables;

    // structs that should not be initialized because they are incomplete
    static std::set<cstring> not_initialized_structs;
    // Properties that define the current state of the program
    // for example, when a return expression must be returned in a block
    static Properties prop;
    // back-end or node-specific restrictions
    static Requirements req;

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
    static std::set<cstring> get_candidate_lvals(const IR::Type *tp,
                                                 bool must_write = true);

    static IR::Type_Bits *pick_declared_bit_type(bool must_write = false);

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

    static std::vector<const IR::Type_Declaration *>
    get_filtered_decls(std::set<cstring> filter);
    static std::set<const IR::P4Table *> *get_callable_tables();
};
} // namespace CODEGEN

#endif // _SCOPE_H_

#include "assignmentOrMethodCallStatement.h"

#include "argument.h"
#include "blockStatement.h"
#include "common.h"
#include "expression.h"
#include "scope.h"

namespace CODEGEN {
IR::Statement *assignmentOrMethodCallStatement::gen_assign() {
    IR::AssignmentStatement *assignstat = nullptr;
    IR::Expression *left = nullptr, *right = nullptr;

    std::vector<int64_t> percent = {
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN_BIT,
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN_STRUCTLIKE};

    switch (randind(percent)) {
    case 0: {
        IR::Type_Bits *bit_type = P4Scope::pick_declared_bit_type(true);
        // Ideally this should have a fallback option
        if (not bit_type) {
            printf("Could not find writable bit lval for assignment!\n");
            // TODO: Find a more meaningful assignment statement
            return nullptr;
        }
        left = P4Scope::pick_lval_or_slice(bit_type);
        right = expression::gen_expr(bit_type);
        return new IR::AssignmentStatement(left, right);
    }
    case 1:
        // TODO: Compound types
        break;
    }

    return assignstat;
}

IR::Statement *gen_methodcall_expression(IR::PathExpression *method_name,
                                         IR::ParameterList params) {
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
    IR::IndexedVector<IR::StatOrDecl> decls;

    // all this boilerplate should be somewhere else...
    P4Scope::start_local_scope();

    for (auto par : params) {
        IR::Argument *arg;
        // TODO: Fix the direction none issue here.
        if (not argument::check_input_arg(par) &&
            par->direction != IR::Direction::None) {
            auto name = randstr(6);
            auto expr = expression::gen_expr(par->type);
            // all this boilerplate should be somewhere else...
            auto decl = new IR::Declaration_Variable(name, par->type, expr);
            P4Scope::add_to_scope(decl);
            decls.push_back(decl);
        }
        arg = new IR::Argument(argument::gen_input_arg(par));
        args->push_back(arg);
    }
    auto mce = new IR::MethodCallExpression(method_name, args);
    auto mcs = new IR::MethodCallStatement(mce);
    P4Scope::end_local_scope();
    if (decls.size() == 0) {
        return mcs;
    } else {
        auto blk_stmt = new IR::BlockStatement(decls);
        blk_stmt->push_back(mcs);
        return blk_stmt;
    }
}

IR::Statement *gen_methodcall(bool is_in_func) {
    IR::MethodCallExpression *mce = nullptr;

    // functions cannot call actions or tables so set their chance to zero
    short tmp_action_pct = PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_ACTION;
    short tmp_tbl_pct = PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_TABLE;
    if (is_in_func) {
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_ACTION = 0;
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_TABLE = 0;
    }
    std::vector<int64_t> percent = {
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_ACTION,
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_FUNCTION,
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_TABLE,
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_CTRL,
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_BUILT_IN};

    switch (randind(percent)) {
    case 0: {
        auto actions = P4Scope::get_decls<IR::P4Action>();
        if (actions.size() == 0) {
            break;
        }
        size_t idx = get_rnd_int(0, actions.size() - 1);
        auto p4_fun = actions.at(idx);
        auto params = p4_fun->getParameters()->parameters;
        auto method_name = new IR::PathExpression(p4_fun->name);
        return gen_methodcall_expression(method_name, params);
    }
    case 1: {
        auto funcs = P4Scope::get_decls<IR::Function>();
        if (funcs.size() == 0) {
            break;
        }
        size_t idx = get_rnd_int(0, funcs.size() - 1);
        auto p4_fun = funcs.at(idx);
        auto params = p4_fun->getParameters()->parameters;
        auto method_name = new IR::PathExpression(p4_fun->name);
        return gen_methodcall_expression(method_name, params);
    }
    case 2: {
        auto tbl_set = P4Scope::get_callable_tables();
        if (tbl_set->size() == 0) {
            break;
        }
        auto idx = get_rnd_int(0, tbl_set->size() - 1);
        auto tbl_iter = std::begin(*tbl_set);
        std::advance(tbl_iter, idx);
        const IR::P4Table *tbl = *tbl_iter;
        auto mem = new IR::Member(new IR::PathExpression(tbl->name), "apply");
        mce = new IR::MethodCallExpression(mem);
        tbl_set->erase(tbl_iter);
        break;
    }
    case 3: {
        auto decls = P4Scope::get_decls<IR::Declaration_Instance>();
        if (decls.size() == 0) {
            break;
        }
        auto idx = get_rnd_int(0, decls.size() - 1);
        auto decl_iter = std::begin(decls);
        std::advance(decl_iter, idx);
        const IR::Declaration_Instance *decl_instance = *decl_iter;
        // avoid member here
        std::stringstream tmp_method_str;
        tmp_method_str << decl_instance->name << ".apply";
        cstring tmp_method_cstr(tmp_method_str.str());
        auto method_name = new IR::PathExpression(tmp_method_cstr);
        auto type_name = decl_instance->type->to<IR::Type_Name>();

        auto resolved_type = P4Scope::get_type_by_name(type_name->path->name);
        if (not resolved_type) {
            BUG("Type Name %s not found", type_name->path->name);
        }
        if (auto ctrl = resolved_type->to<IR::P4Control>()) {
            auto params = ctrl->getApplyParameters()->parameters;
            return gen_methodcall_expression(method_name, params);
        }
        BUG("Declaration Instance type %s not yet supported",
            decl_instance->type->node_type_name());
    }
    case 4: {
        auto hdrs = P4Scope::get_decls<IR::Type_Header>();
        if (hdrs.size() == 0) {
            break;
        }
        std::set<cstring> hdr_lvals;
        for (auto hdr : hdrs) {
            auto available_lvals = P4Scope::get_candidate_lvals(hdr, true);
            hdr_lvals.insert(available_lvals.begin(), available_lvals.end());
        }
        if (hdr_lvals.size() == 0) {
            break;
        }
        auto idx = get_rnd_int(0, hdr_lvals.size() - 1);
        auto hdr_lval_iter = std::begin(hdr_lvals);
        std::advance(hdr_lval_iter, idx);
        cstring hdr_lval = *hdr_lval_iter;
        cstring call;
        if (get_rnd_int(0, 1)) {
            call = "setValid";
        } else {
            call = "setInvalid";
        }
        auto mem = new IR::Member(new IR::PathExpression(hdr_lval), call);
        mce = new IR::MethodCallExpression(mem);
        break;
    }
    }
    // restore previous probabilities
    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_ACTION = tmp_action_pct;
    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_TABLE = tmp_tbl_pct;
    if (mce) {
        return new IR::MethodCallStatement(mce);
    } else {
        // unable to return a methodcall, return an assignment instead
        return assignmentOrMethodCallStatement::gen_assign();
    }
}

IR::Statement *assignmentOrMethodCallStatement::gen(bool is_in_func) {
    std::vector<int64_t> percent = {
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN,
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_CALL};
    auto val = randind(percent);
    if (val == 0) {
        return assignmentOrMethodCallStatement::gen_assign();
    } else {
        return gen_methodcall(is_in_func);
    }
}
} // namespace CODEGEN

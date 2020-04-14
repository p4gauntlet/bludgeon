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

    std::vector<int> percent = {100, 0};

    switch (randind(percent)) {
    case 0: {
        IR::Type_Bits *bit_type = P4Scope::pick_declared_bit_type(true);
        // Ideally this should have a fallback option
        if (not bit_type) {
            printf("Could not find writable bit lval for assignment!\n");
            // TODO: Find a more meaningful assignment statement
            return nullptr;
        }
        cstring name = P4Scope::pick_lval(bit_type, true);
        left = new IR::PathExpression(name);
        right = expression::gen_expr(bit_type);
        return new IR::AssignmentStatement(left, right);
    }
    case 1:
        // TODO: Compound types
        break;
    }

    return assignstat;
}

IR::Statement *gen_methodcall_expression(cstring method_name,
                                         IR::ParameterList params) {
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
    IR::IndexedVector<IR::StatOrDecl> decls;

    // all this boilerplate should be somewhere else...
    P4Scope::start_local_scope();

    for (auto par : params) {
        IR::Argument *arg;
        if (not argument::check_input_arg(par)) {
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
    auto path_expr = new IR::PathExpression(method_name);
    auto mce = new IR::MethodCallExpression(path_expr, args);
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

    int fun_pct = 40;
    int action_pct = 40;
    int tbl_pct = 15;
    int built_in = 5;
    // functions cannot call actions or tables so set their chance to zero
    if (is_in_func) {
        action_pct = 0;
        tbl_pct = 0;
    }
    std::vector<int> percent = {action_pct, fun_pct, tbl_pct, built_in};

    switch (randind(percent)) {
    case 0: {
        auto actions = P4Scope::get_decls<IR::P4Action>();
        if (actions.size() == 0) {
            break;
        }
        size_t idx = rand() % actions.size();
        auto p4_fun = actions.at(idx);
        auto params = p4_fun->getParameters()->parameters;
        return gen_methodcall_expression(p4_fun->name, params);
    }
    case 1: {
        auto funcs = P4Scope::get_decls<IR::Function>();
        if (funcs.size() == 0) {
            break;
        }
        size_t idx = rand() % funcs.size();
        auto p4_fun = funcs.at(idx);
        auto params = p4_fun->getParameters()->parameters;
        return gen_methodcall_expression(p4_fun->name, params);
    }
    case 2: {
        auto tbl_set = P4Scope::get_callable_tables();
        if (tbl_set->size() == 0) {
            break;
        }
        auto idx = rand() % tbl_set->size();
        auto tbl_iter = std::begin(*tbl_set);
        std::advance(tbl_iter, idx);
        const IR::P4Table *tbl = *tbl_iter;
        auto mem = new IR::Member(new IR::PathExpression(tbl->name), "apply");
        mce = new IR::MethodCallExpression(mem);
        tbl_set->erase(tbl_iter);
        break;
    }
    case 3: {
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
        auto idx = rand() % hdr_lvals.size();
        auto hdr_lval_iter = std::begin(hdr_lvals);
        std::advance(hdr_lval_iter, idx);
        cstring hdr_lval = *hdr_lval_iter;
        cstring call;
        if (rand() % 2) {
            call = "setValid";
        } else {
            call = "setInvalid";
        }
        auto mem = new IR::Member(new IR::PathExpression(hdr_lval), call);
        mce = new IR::MethodCallExpression(mem);
        break;
    }
    }
    if (mce) {
        return new IR::MethodCallStatement(mce);
    } else {
        // unable to return a methodcall, return an assignment instead
        return assignmentOrMethodCallStatement::gen_assign();
    }
}

IR::Statement *assignmentOrMethodCallStatement::gen(bool is_in_func) {
    std::vector<int> percent = {75, 25};
    auto val = randind(percent);
    if (val == 0) {
        return assignmentOrMethodCallStatement::gen_assign();
    } else {
        return gen_methodcall(is_in_func);
    }
}
} // namespace CODEGEN

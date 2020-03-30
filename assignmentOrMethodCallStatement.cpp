#include "assignmentOrMethodCallStatement.h"
#include "expression_2.h"

namespace CODEGEN {
IR::AssignmentStatement *assignmentOrMethodCallStatement::gen_assign() {
    IR::AssignmentStatement *assignstat = nullptr;
    IR::Expression *left = nullptr, *right = nullptr;
    const IR::Type *l_tp, *r_tp;

    std::vector<int> percent = { 75, 25 };

    switch (randind(percent)) {
        case 0:
            left = expression::get_bit_operand(&l_tp, false);
            if (left != nullptr) {
                right      = expression2::gen_expr(l_tp->to<IR::Type_Bits>());
                assignstat = new IR::AssignmentStatement(left, right);
            }
            break;
        case 1:
            // compund means it is not a simple operator, i.e., bit<128> a is simple, compound may be struct, header
            if (expression::get_two_compound_operands(&left, &right,
                                                      &l_tp,
                                                      &r_tp) == true) {
                assignstat = new IR::AssignmentStatement(left, right);
            }
            break;
    }


    return assignstat;
}


IR::Statement *gen_methodcall() {
    IR::MethodCallExpression *mce = nullptr;
    std::vector<int> percent      = { 40, 40, 20 };

    switch (randind(percent)) {
        case 0: {
                auto actions = P4Scope::get_action_decls();
                if (actions.size() == 0) {
                    return nullptr;
                }

                size_t idx   = rand() % actions.size();
                auto *p4_fun = actions.at(idx);
                std::vector<const IR::Type *> params;
                for (auto par: p4_fun->getParameters()->parameters) {
                    params.push_back(par->type);
                }
                IR::Vector<IR::Argument> *args =
                    expression::construct_params(params);
                if (args->size() != params.size()) {
                    return nullptr;
                }
                auto path_expr = new IR::PathExpression(p4_fun->name);
                mce = new IR::MethodCallExpression(path_expr, args);
                break;
            }
        case 1: {
                auto funcs = P4Scope::get_func_decls();
                if (funcs.size() == 0) {
                    return nullptr;
                }

                size_t idx   = rand() % funcs.size();
                auto *p4_fun = funcs.at(idx);
                std::vector<const IR::Type *> params;
                for (auto par: p4_fun->getParameters()->parameters) {
                    params.push_back(par->type);
                }
                IR::Vector<IR::Argument> *args =
                    expression::construct_params(params);
                if (args->size() != params.size()) {
                    return nullptr;
                }
                auto path_expr = new IR::PathExpression(p4_fun->name);
                mce = new IR::MethodCallExpression(path_expr, args);
                break;
            }
        case 2: {
                auto tbl_set = P4Scope::get_callable_tables();
                // return nullptr if there are no tables left
                if (tbl_set->size() == 0) {
                    return nullptr;
                }
                auto idx      = rand() % tbl_set->size();
                auto tbl_iter = std::begin(*tbl_set);
                std::advance(tbl_iter, idx);
                const IR::P4Table *tbl = *tbl_iter;
                auto mem =
                    new IR::Member(new IR::PathExpression(tbl->name),
                                   "apply");
                mce = new IR::MethodCallExpression(mem);
                tbl_set->erase(tbl_iter);
                break;
            }
    }
    if (mce) {
        return new IR::MethodCallStatement(mce);
    } else{
        return nullptr;
    }
}


IR::Statement *assignmentOrMethodCallStatement::gen() {
    std::vector<int> percent = { 50, 50 };
    auto val = randind(percent);

    if (val == 0) {
        return assignmentOrMethodCallStatement::gen_assign();
    } else {
        return gen_methodcall();
    }
}
} // namespace CODEGEN

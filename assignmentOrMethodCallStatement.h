#ifndef _ASSIGNMENTORMETHODCALLSTATEMENT_H_
#define _ASSIGNMENTORMETHODCALLSTATEMENT_H_

#include "ir/ir.h"


#include "expression.h"

namespace CODEGEN {


class assignmentOrMethodCallStatement {
public:
	const char* types[0] = {
	};

	assignmentOrMethodCallStatement() {
	}

	static IR::AssignmentStatement* gen_assignstat() {
		IR::AssignmentStatement *assignstat = nullptr;
		IR::Expression *left=nullptr, *right=nullptr;
		const IR::Type *l_tp, *r_tp;
		left = expression::get_bit_operand(&l_tp);
		right = expression::construct_op_expr();
		if (left!=nullptr && right!=nullptr) {
			r_tp = expression::mp_expr_2_type[right];
			int l_size = l_tp->to<IR::Type_Bits>()->size;
			int r_size = r_tp->to<IR::Type_Bits>()->size;
			if (l_size != r_size) {
				assignstat = new IR::AssignmentStatement(left, 
						new IR::Cast(new IR::Type_Bits(l_size, false), right));
			}
			else {
				assignstat = new IR::AssignmentStatement(left, right);
			}
		}

		return assignstat;
	}

	static IR::AssignmentStatement* gen_compound_ass() {
		IR::AssignmentStatement *assignstat = nullptr;
		IR::Expression* expr1, * expr2;
		const IR::Type* tp1;
		const IR::Type* tp2;
		if (expression::get_two_compound_operands(&expr1, &expr2, &tp1, &tp2) == true) {
			assignstat = new IR::AssignmentStatement(expr1, expr2);
		}

		return assignstat;
	}

	// TODO: methodcall statement is a big headache
	static IR::MethodCallStatement* gen_methodcall_stat() {
		IR::MethodCallStatement* mcs = nullptr;

		if (P4Scope::decl_ins_ctrls.size() == 0) {
			return nullptr;
		}

		size_t ind = rand()%P4Scope::decl_ins_ctrls.size();
		size_t cnt = 0;
		cstring name;
		IR::P4Control* p4_ctrl;
		for (auto &i : P4Scope::decl_ins_ctrls) {
			if (cnt == ind) {
				name = i.first;
				p4_ctrl = i.second;
			}
			cnt++;
		}
		std::vector<const IR::Type *> params;
		for (size_t i=0; i<p4_ctrl->type->applyParams->parameters.size(); i++) {
			auto par = p4_ctrl->type->applyParams->parameters.at(i);
			params.push_back(par->type);
		}
		IR::Vector<IR::Argument> * args = expression::construct_params(params);
		if (args->size() != params.size()) {
			return nullptr;
		}
		IR::ID apply("apply");
		IR::ID call_name(name);
		IR::Member * mem = new IR::Member(new IR::PathExpression(new IR::Path(call_name)), apply);
		mcs = new IR::MethodCallStatement(new IR::MethodCallExpression(mem, args));

		return mcs;
	}

	static IR::MethodCallStatement* gen_act_methodcall_stat() {
		IR::MethodCallStatement* mcs = nullptr;

		if (P4Scope::decl_actions.size() == 0) {
			return nullptr;
		}

		size_t ind = rand()%P4Scope::decl_actions.size();
		size_t cnt = 0;
		cstring name;
		IR::P4Action* p4_act;
		for (auto &i : P4Scope::decl_actions) {
			if (cnt == ind) {
				name = i.first;
				p4_act = i.second;
			}
			cnt++;
		}
		std::vector<const IR::Type *> params;
		for (size_t i=0; i<p4_act->parameters->parameters.size(); i++) {
			auto par = p4_act->parameters->parameters.at(i);
			params.push_back(par->type);
		}
		IR::Vector<IR::Argument> * args = expression::construct_params(params);
		if (args->size() != params.size()) {
			return nullptr;
		}
		IR::ID call_name(name);
		auto path_expr = new IR::PathExpression(new IR::Path(call_name));
		mcs = new IR::MethodCallStatement(new IR::MethodCallExpression(path_expr, args));

		return mcs;
	}


};


} // namespace CODEGEN




#endif

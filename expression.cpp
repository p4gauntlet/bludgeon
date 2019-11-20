


#include "expression.h"

namespace CODEGEN {

std::vector<IR::Expression *> expression::boolean_exprs;
std::map<IR::Expression *, const IR::Type*> expression::mp_expr_2_type;
std::vector<IR::Expression *> expression::bit_exprs;

const IR::Type* pick_field(std::map<cstring, const IR::Type*> &mp,
        const IR::Type* tp, std::vector<cstring> &call_bt, cstring &type);

// -1: not a int, otherwise: int value;
int check_if_int(cstring &str) {
	int ret = -1;
	bool is_int = true;
	std::stringstream ss;
	ss << str;
	std::string s = ss.str();

	for (auto it=s.begin(); it!=s.end(); it++) {
		if (!std::isdigit(*it)) {
			is_int = false;
			break;
		}
	}

	if (is_int == true) {
		ret = std::stoi(s);
	}

	return ret;
}

// pick a lvalue
const IR::Type* pick_lval(std::map<cstring, const IR::Type*> &mp,
        std::vector<cstring> &call_bt, cstring &type) {
    size_t cnt = 0;
    cstring name;
    const IR::Type* tp = nullptr;
    for (auto &k : mp) {
        name = k.first;

        cnt++;
        if (rand()%2==0 || cnt==mp.size()) {
            tp = k.second;
            break;
        }
    }

    if (tp == nullptr) {
        return tp;
    }

    call_bt.push_back(name);

    return pick_field(mp, tp, call_bt, type);
}

// pick field of a var or param
const IR::Type* pick_field(std::map<cstring, const IR::Type*> &mp,
        const IR::Type* tp, std::vector<cstring> &call_bt, cstring &type) {
    if (tp == nullptr) {
        return tp;
    }

    if (tp->is<IR::Type_StructLike>()) {
        const IR::Type_StructLike* t = tp->to<IR::Type_StructLike>();
        // call_bt.push_back(t->name.name);
        size_t cnt = 0;
        const IR::StructField* sf = nullptr;
        for (size_t i=0; i<t->fields.size(); i++) {
            cnt++;
            if (rand()%2==0 || cnt==t->fields.size()) {
                sf = t->fields.at(i);
                break;
            }
        }
        return pick_field(mp, (IR::Type *)sf, call_bt, type);
    }
    else if (tp->is<IR::StructField>()) {
        const IR::StructField* t = tp->to<IR::StructField>();
        call_bt.push_back(t->name.name);
        return pick_field(mp, t->type, call_bt, type);
    }
    else if (tp->is<IR::Type_Stack>()) {
        std::stringstream ss;
        const IR::Type_Stack* t = tp->to<IR::Type_Stack>();
        ss << rand()%t->getSize();
        cstring name(ss.str());
        call_bt.push_back(name);
        return pick_field(mp, t->elementType, call_bt, type);
    }
    else if (tp->is<IR::Type_Name>()) {
        const IR::Type_Name* t = tp->to<IR::Type_Name>();
        const IR::Type* ref_tp = P4Scope::get_type_by_name(t->path->name.name);
		if (ref_tp == nullptr) {
			BUG("nullptr not possible");
		}
        return pick_field(mp, ref_tp, call_bt, type);
    }
	else if (tp->is<IR::Type_Typedef>()) {
		const IR::Type_Typedef* t = tp->to<IR::Type_Typedef>();
		const IR::Type* ref_tp = t->type;
		if (ref_tp == nullptr) {
			BUG("nullptr not possible");
		}
		return pick_field(mp, ref_tp, call_bt, type);
	}
	// Tao: final types, either type_bits or type_boolean
    else if (tp->is<IR::Type_Bits>()) {
        // const IR::Type_Bits* t = tp->to<IR::Type_Bits>();
        type = EXPR_TYPE_BITS;
        return tp;
    }
    else if (tp->is<IR::Type_Boolean>()) {
        // const IR::Type_Boolean* t = tp->to<IR::Type_Boolean>();
        type = EXPR_TYPE_BOOL;
        return tp;
    }

	return nullptr;
}

IR::Expression* construct_expr(std::vector<cstring> &call_bt) {
	IR::Expression* expr;

	size_t cnt = 0;
	for(auto i=call_bt.begin(); i<call_bt.end(); i++) {
		if (cnt == 0) {
			IR::ID name(*i);
			expr = new IR::PathExpression(new IR::Path(name));
		}
		else {
			int ret = check_if_int((*i));
			if (ret == -1) {
				// not int, then IR::Member
				expr = new IR::Member(expr, IR::ID((*i)));
			}
			else {
				// if int, the IR::ArrayIndex
				expr = new IR::ArrayIndex(expr, new IR::Constant(int_literal::gen(), ret));
			}
		}
		cnt = cnt+1;
	}

	return expr;
}

IR::Expression* expression::get_operand(int pa_or_var, const IR::Type** tp, cstring &type) {
    std::vector<cstring> call_backtrace;

	if (pa_or_var == 0) {
		*tp = pick_lval(P4Scope::name_2_type_param, call_backtrace, type);
	}
	else {
		*tp = pick_lval(P4Scope::name_2_type_vars, call_backtrace, type);
	}

	if (*tp == nullptr) {
		return nullptr;
	}

	return construct_expr(call_backtrace);
}

IR::Expression* expression::get_bit_operand(const IR::Type** tp) {
	IR::Expression* expr = nullptr;
	cstring type;
	int num_trials = 100;
	while (num_trials--) {
		expr = get_operand(rand()%2, tp, type);
		if (expr!=nullptr && type==EXPR_TYPE_BITS) {
			break;
		}
	}

	return expr;
}

IR::Expression* expression::get_op_expr() {
	IR::Expression* expr = nullptr;
	const IR::Type *tp = nullptr;
	bool is_from_vector = false;
	if (bit_exprs.size() != 0) {
		size_t ind = rand()%bit_exprs.size();
		expr = bit_exprs.at(ind);
		bit_exprs.erase(bit_exprs.begin()+ind);
		is_from_vector = true;
	}
	else {
		expr = get_bit_operand(&tp);
	}

	if (expr == nullptr) {
		return nullptr;
	}

	if (is_from_vector == false) {
		mp_expr_2_type[expr] = tp;
	}

	return expr;
}

IR::Expression* expression::construct_l_val() {
}

IR::Expression* expression::construct_op_expr() {
	IR::Expression *expr;
	IR::Expression *expr1, *expr2;
	expr1 = get_op_expr();
	expr2 = get_op_expr();
	if (expr1==nullptr || expr2==nullptr) {
		return nullptr;
	}

	switch(2) {
		case 0: expr = new IR::Cmpl(expr1); mp_expr_2_type[expr] = mp_expr_2_type[expr1]; break;
		case 1: expr = new IR::Neg(expr1); mp_expr_2_type[expr] = mp_expr_2_type[expr2]; break;
		case 2: {
					auto tp = mp_expr_2_type[expr1];
					int size = tp->to<IR::Type_Bits>()->size;
					int r_range = rand()%size;
					int l_range = rand()%(size-r_range)+r_range;
					expr = new IR::Slice(expr1, new IR::Constant(l_range),
											new IR::Constant(r_range));
				}
	}



	return expr;
}

IR::Expression* expression::get_cond_expr() {
	IR::Expression* expr = nullptr;
	const IR::Type* tp = nullptr;
	cstring type;
	bool is_from_vector = false;
	if (boolean_exprs.size() != 0) {
		size_t ind = rand()%boolean_exprs.size();
		expr = boolean_exprs.at(ind);
		boolean_exprs.erase(boolean_exprs.begin()+ind);
		is_from_vector = true;
	}
	else {
		int num_trials = 100;
		while (num_trials--) {
			expr = get_operand(rand()%2, &tp, type);
			if (expr != nullptr) {
				break;
			}
		}
	}

	if (expr == nullptr) {
		return nullptr;
	}

	if (is_from_vector == false) {
		if (type == EXPR_TYPE_BOOL) {
			switch(rand()%6) {
			case 0: expr = new IR::Equ(expr, bool_literal::gen_literal()); break;
			case 1: expr = new IR::Geq(expr, bool_literal::gen_literal()); break;
			case 2: expr = new IR::Grt(expr, bool_literal::gen_literal()); break;
			case 3: expr = new IR::Leq(expr, bool_literal::gen_literal()); break;
			case 4: expr = new IR::Lss(expr, bool_literal::gen_literal()); break;
			case 5: expr = new IR::Neq(expr, bool_literal::gen_literal()); break;
			}
		}
		else if (type == EXPR_TYPE_BITS) {
			switch(rand()%6) {
			case 0: expr = new IR::Equ(expr, bit_literal::gen_literal()); break;
			case 1: expr = new IR::Geq(expr, bit_literal::gen_literal()); break;
			case 2: expr = new IR::Grt(expr, bit_literal::gen_literal()); break;
			case 3: expr = new IR::Leq(expr, bit_literal::gen_literal()); break;
			case 4: expr = new IR::Lss(expr, bit_literal::gen_literal()); break;
			case 5: expr = new IR::Neq(expr, bit_literal::gen_literal()); break;
			}
		}
	}

	return expr;
}

IR::Expression* expression::construct_cond_expr() {
	IR::Expression *expr = nullptr;
	IR::Expression *expr1, *expr2;
	expr1 = get_cond_expr();
	expr2 = get_cond_expr();
	if (expr1==nullptr || expr2==nullptr) {
		return nullptr;
	}

	switch (rand()%3) {
		case 0: expr = new IR::LNot(expr1); break;
		case 1: expr = new IR::LAnd(expr1, expr2); break;
		case 2: expr = new IR::LOr(expr1, expr2); break;
	}

	if (expr != nullptr) {
		boolean_exprs.push_back(expr);
	}


	return expr;
}


} // namespace CODEGEN


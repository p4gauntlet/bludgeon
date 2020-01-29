


#include "expression.h"

namespace CODEGEN {

std::vector<IR::Expression *> expression::boolean_exprs;
std::map<IR::Expression *, const IR::Type*> expression::mp_expr_2_type;
std::map<IR::Expression *, bool> mp_expr_2_if_inc_in;
std::vector<IR::Expression *> expression::bit_exprs;
std::set<IR::Expression *> expression::forbidden_exprs;

const IR::Type* pick_field(std::map<cstring, const IR::Type*> &mp,
        const IR::Type* tp, std::vector<cstring> &call_bt, cstring &type, bool if_compound);

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
        std::vector<cstring> &call_bt, cstring &type, bool if_compound) {
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

    return pick_field(mp, tp, call_bt, type, if_compound);
}

// pick field of a var or param
const IR::Type* pick_field(std::map<cstring, const IR::Type*> &mp,
        const IR::Type* tp, std::vector<cstring> &call_bt, cstring &type, bool if_compound) {
    if (tp == nullptr) {
        return tp;
    }

    if (tp->is<IR::Type_StructLike>()) {
		if (if_compound == true) {
			if (tp->is<IR::Type_Struct>()) {
				type = EXPR_TYPE_STRUCT;
			}
			else if (tp->is<IR::Type_Header>()) {
				type = EXPR_TYPE_HEADER;
			}
			else if (tp->is<IR::Type_HeaderUnion>()) {
				type = EXPR_TYPE_HUNION;
			}
			return tp;
		}
        const IR::Type_StructLike* t = tp->to<IR::Type_StructLike>();
        size_t cnt = 0;
        const IR::StructField* sf = nullptr;
        for (size_t i=0; i<t->fields.size(); i++) {
            cnt++;
            if (rand()%2==0 || cnt==t->fields.size()) {
                sf = t->fields.at(i);
                break;
            }
        }
        return pick_field(mp, (IR::Type *)sf, call_bt, type, if_compound);
    }
    else if (tp->is<IR::StructField>()) {
        const IR::StructField* t = tp->to<IR::StructField>();
        call_bt.push_back(t->name.name);
        return pick_field(mp, t->type, call_bt, type, if_compound);
    }
    else if (tp->is<IR::Type_Stack>()) {
        std::stringstream ss;
        const IR::Type_Stack* t = tp->to<IR::Type_Stack>();
        ss << rand()%t->getSize();
        cstring name(ss.str());
        call_bt.push_back(name);
        return pick_field(mp, t->elementType, call_bt, type, if_compound);
    }
    else if (tp->is<IR::Type_Name>()) {
        const IR::Type_Name* t = tp->to<IR::Type_Name>();
        const IR::Type* ref_tp = P4Scope::get_type_by_name(t->path->name.name);
		if (ref_tp == nullptr) {
			// BUG("nullptr not possible");
		}
        return pick_field(mp, ref_tp, call_bt, type, if_compound);
    }
	else if (tp->is<IR::Type_Typedef>()) {
		const IR::Type_Typedef* t = tp->to<IR::Type_Typedef>();
		const IR::Type* ref_tp = t->type;
		if (ref_tp == nullptr) {
			// BUG("nullptr not possible");
		}
		return pick_field(mp, ref_tp, call_bt, type, if_compound);
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

void expression::initialize(const IR::Type* tp,
		std::vector<IR::AssignmentStatement*> &ass_stat, 
		std::vector<cstring> &call_bt) {
	if (tp->is<IR::Type_StructLike>()) {
		const IR::Type_StructLike* t = tp->to<IR::Type_StructLike>();
		const IR::StructField* sf = nullptr;
		for (size_t i=0; i<t->fields.size(); i++) {
			sf = t->fields.at(i);
			initialize((IR::Type *)sf, ass_stat, call_bt);
		}
	}
	else if (tp->is<IR::StructField>()) {
		const IR::StructField* t = tp->to<IR::StructField>();
		call_bt.push_back(t->name.name);
		initialize(t->type, ass_stat, call_bt);
		call_bt.pop_back();
	}
	else if (tp->is<IR::Type_Stack>()) {
		std::stringstream ss;
		const IR::Type_Stack* t = tp->to<IR::Type_Stack>();
		for (unsigned i=0; i<t->getSize(); i++) {
			ss.str("");
			ss << i;
			cstring name(ss.str());
			call_bt.push_back(name);
			initialize(t->elementType, ass_stat, call_bt);
			call_bt.pop_back();
		}
	}
	else if (tp->is<IR::Type_Name>()) {
		const IR::Type_Name* t = tp->to<IR::Type_Name>();
		const IR::Type* ref_tp = P4Scope::get_type_by_name(t->path->name.name);
		if (ref_tp == nullptr) {
			BUG("nullptr not possible");
		}
		initialize(ref_tp, ass_stat, call_bt);
	}
	else if (tp->is<IR::Type_Typedef>()) {
		const IR::Type_Typedef* t = tp->to<IR::Type_Typedef>();
		const IR::Type* ref_tp = t->type;
		if (ref_tp == nullptr) {
			BUG("nullptr not possible");
		}
		initialize(ref_tp, ass_stat, call_bt);
	}
	else if (tp->is<IR::Type_Bits>()) {
		IR::Expression* l_expr = construct_expr(call_bt);
		auto tp_bits = tp->to<IR::Type_Bits>();
		int size = (tp_bits->size<=SIZE_BIT_FOR_INITIALIZATION)?
					tp_bits->size:SIZE_BIT_FOR_INITIALIZATION; // Tao: watch out for overflow
		IR::Constant* r_expr = new IR::Constant(tp_bits, rand()%(2<<(size-1)));
		IR::AssignmentStatement* ass = new IR::AssignmentStatement(l_expr, r_expr);
		ass_stat.push_back(ass);
	}
	else if (tp->is<IR::Type_Boolean>()) {
		IR::Expression* l_expr = construct_expr(call_bt);
		IR::BoolLiteral* r_expr = new IR::BoolLiteral(false);
		IR::AssignmentStatement* ass = new IR::AssignmentStatement(l_expr, r_expr);
		ass_stat.push_back(ass);
	}

	return ;
}

std::vector<IR::AssignmentStatement*> expression::decl_initialize(std::map<cstring, const IR::Type*> &tp_map) {
	std::vector<IR::AssignmentStatement*> ass_stats;
	std::vector<cstring> call_bt;
	cstring name;
	const IR::Type *tp = nullptr;
	for (auto &k : tp_map) {
		name = k.first;
		tp = k.second;
		call_bt.push_back(name);
		initialize(tp, ass_stats, call_bt);
		call_bt.pop_back();
	}
	return ass_stats;
}

std::vector<IR::AssignmentStatement*> expression::decl_p_initialize() {
	return decl_initialize(P4Scope::name_2_type_param);
}

std::vector<IR::AssignmentStatement*> expression::decl_v_initialize() {
	return decl_initialize(P4Scope::name_2_type_stack);
}

IR::Expression* expression::get_operand(int pa_or_var, const IR::Type** tp, cstring &type, bool if_compound, bool if_param_in_inc) {
    std::vector<cstring> call_backtrace;

	if (pa_or_var == 0) {
		if (if_param_in_inc == true) {
			*tp = pick_lval(P4Scope::name_2_type_param_in, call_backtrace, type, if_compound);
		}
		else {
			*tp = pick_lval(P4Scope::name_2_type_param, call_backtrace, type, if_compound);
		}
	}
	else {
		*tp = pick_lval(P4Scope::name_2_type_vars, call_backtrace, type, if_compound);
	}

	if (*tp == nullptr) {
		return nullptr;
	}

	return construct_expr(call_backtrace);
}


IR::Expression* expression::get_bit_operand(const IR::Type** tp, bool if_param_in_inc=false) {
	IR::Expression* expr = nullptr;
	cstring type;
	int num_trials = 100;
	while (num_trials--) {
		expr = get_operand(rand()%2, tp, type, false, if_param_in_inc);
		if (expr!=nullptr && type==EXPR_TYPE_BITS) {
			break;
		}
	}

	return expr;
}

IR::Expression* expression::get_op_expr(bool if_param_in_inc=false) {
	IR::Expression* expr = nullptr;
	const IR::Type *tp = nullptr;
	bool is_from_vector = false;
	if (bit_exprs.size()!=0 && rand()%2==0) {
		size_t ind = rand()%bit_exprs.size();
		expr = bit_exprs.at(ind);
		bit_exprs.erase(bit_exprs.begin()+ind);
		is_from_vector = true;
	}
	else {
		expr = get_bit_operand(&tp, if_param_in_inc);
	}

	if (expr == nullptr) {
		return nullptr;
	}

	if (is_from_vector == false) {
		mp_expr_2_type[expr] = tp;
	}

	return expr;
}

bool expression::get_list_expressions(IR::Vector<IR::Expression> &exprs,
                                std::vector<const IR::Type*> &types,
                                size_t num) {
    bool ret = true;
    for (size_t i=0; i<num; i++) {
        auto expr = get_op_expr();
        if (expr == nullptr) {
            ret = false;
            break;
        }
        exprs.push_back(expr);
        types.push_back(mp_expr_2_type[expr]);
    }

    return ret;
}



IR::Expression* construct_slice(IR::Expression* expr1) {
	IR::Expression* expr = nullptr;
	if (expression::forbidden_exprs.find(expr1) != expression::forbidden_exprs.end()) {
		return nullptr;
	}
	
	auto tp = expression::mp_expr_2_type[expr1];
	int size = tp->to<IR::Type_Bits>()->size;
	if (size == 0) {
		return nullptr;
	}
	int r_range = rand()%size;
	int l_range = rand()%(size-r_range)+r_range;
	expr = new IR::Slice(expr1, new IR::Constant(l_range),
							new IR::Constant(r_range));
	expression::mp_expr_2_type.emplace(expr, new IR::Type_Bits(l_range-r_range+1, false));
	expression::bit_exprs.push_back(expr);

	return expr;
}

IR::Expression* construct_mux(IR::Expression* expr1, IR::Expression* expr2) {
	IR::Expression* expr = nullptr;
	if (expression::forbidden_exprs.find(expr1) != expression::forbidden_exprs.end() ||
			expression::forbidden_exprs.find(expr2) != expression::forbidden_exprs.end()) {
		return nullptr;
	}
	auto tp1 = expression::mp_expr_2_type[expr1];
	auto tp2 = expression::mp_expr_2_type[expr2];
	int size1 = tp1->to<IR::Type_Bits>()->size;
	int size2 = tp2->to<IR::Type_Bits>()->size;
	int size = size1>size2?size1:size2;
	IR::Expression *ca_expr1, *ca_expr2;
	if (size > size1) {
		ca_expr1 = new IR::Cast(new IR::Type_Bits(size, false), expr1);
		expr = new IR::Mux(expression::get_cond_expr(), ca_expr1, expr2);
	}
	if (size > size2) {
		ca_expr2 = new IR::Cast(new IR::Type_Bits(size, false), expr2);
		expr = new IR::Mux(expression::get_cond_expr(), expr1, ca_expr2);
	}

	expression::mp_expr_2_type.emplace(expr, new IR::Type_Bits(size, false));
	expression::bit_exprs.push_back(expr);

	return expr;
}

template<typename T>
IR::Expression* construct_2_ops(IR::Expression* expr1, IR::Expression* expr2, bool if_cast=true) {
	IR::Expression* expr = nullptr;
	if (expression::forbidden_exprs.find(expr1) != expression::forbidden_exprs.end() ||
			expression::forbidden_exprs.find(expr2) != expression::forbidden_exprs.end()) {
		return nullptr;
	}
	if (if_cast == true) {
		auto tp1 = expression::mp_expr_2_type[expr1];
		auto tp2 = expression::mp_expr_2_type[expr2];
		int size1 = tp1->to<IR::Type_Bits>()->size;
		int size2 = tp2->to<IR::Type_Bits>()->size;
		int size = size1>size2?size1:size2;
		IR::Expression *ca_expr1, *ca_expr2;
		if (size > size1) {
			ca_expr1 = new IR::Cast(new IR::Type_Bits(size, false), expr1);
			expr = new T(ca_expr1, expr2);
		}
		if (size > size2) {
			ca_expr2 = new IR::Cast(new IR::Type_Bits(size, false), expr2);
			expr = new T(expr1, ca_expr2);
		}

		expression::mp_expr_2_type.emplace(expr, new IR::Type_Bits(size, false));
	}
	else {
		expr = new T(expr1, expr2);
		expression::mp_expr_2_type[expr] = expression::mp_expr_2_type[expr1];
	}
	expression::bit_exprs.push_back(expr);

	return expr;
}

IR::Expression* construct_div(IR::Expression* expr1) {
	IR::Expression* expr = nullptr;
	const IR::Type* tp = expression::mp_expr_2_type[expr1];
	int size = tp->to<IR::Type_Bits>()->size;
	int val = 1<<(rand()%(size<=SIZE_BIT_FOR_INITIALIZATION?size:SIZE_BIT_FOR_INITIALIZATION));
	expr = new IR::Div(expr1, new IR::Constant(new IR::Type_Bits(size, false), val));
	expression::mp_expr_2_type[expr] = expression::mp_expr_2_type[expr1];
	expression::bit_exprs.push_back(expr);
	return expr;
}

IR::Expression* construct_mod(IR::Expression* expr1) {
	IR::Expression* expr = nullptr;
	const IR::Type* tp = expression::mp_expr_2_type[expr1];
	int size = tp->to<IR::Type_Bits>()->size;
	expr = new IR::Mod(expr1, new IR::Constant(new IR::Type_Bits(size, false), rand()%size+1));
	expression::mp_expr_2_type[expr] = expression::mp_expr_2_type[expr1];
	expression::bit_exprs.push_back(expr);
	return expr;
}

IR::Expression* expression::construct_op_expr() {
	IR::Expression *expr;
	IR::Expression *expr1, *expr2;
	expr1 = get_op_expr(true);
	expr2 = get_op_expr(true);
	if (expr1==nullptr || expr2==nullptr) {
		return nullptr;
	}

	int num_trials = 100;
	while (num_trials--) {
		switch(rand()%15) {
			case 0: expr = new IR::Cmpl(expr1); mp_expr_2_type[expr] = mp_expr_2_type[expr1]; bit_exprs.push_back(expr); break;
			case 1: expr = new IR::Neg(expr1); mp_expr_2_type[expr] = mp_expr_2_type[expr1]; bit_exprs.push_back(expr); break;
			case 2: expr = construct_slice(expr1); break;
			case 3: expr = construct_2_ops<IR::Mul>(expr1, expr2); break;
			case 4: expr = construct_div(expr1); break;
			case 5: expr = construct_2_ops<IR::Add>(expr1, expr2); break;
			case 6: expr = construct_2_ops<IR::Sub>(expr1, expr2); break;
			case 7: expr = construct_2_ops<IR::AddSat>(expr1, expr2); break;
			case 8: expr = construct_2_ops<IR::SubSat>(expr1, expr2); break;
			case 9: expr = construct_2_ops<IR::BAnd>(expr1, expr2); break;
			case 10: expr = construct_2_ops<IR::BOr>(expr1, expr2); break;
			case 11: expr = construct_2_ops<IR::BXor>(expr1, expr2); break;
			case 12: expr = construct_2_ops<IR::Shr>(expr1, 
							 new IR::Cast(new IR::Type_Bits(8, false), expr2)
							 , false); break;
			case 13: expr = construct_2_ops<IR::Shl>(expr1,
							 new IR::Cast(new IR::Type_Bits(8, false), expr2)
							 , false); break;
			case 14: expr = construct_mux(expr1, expr2); break;
			// case 15: expr = construct_mod(expr1); break;
		}

		if (expr != nullptr) {
			break;
		}
	}



	return expr;
}

IR::Expression* expression::get_cond_expr() {
	IR::Expression* expr = nullptr;
	const IR::Type* tp = nullptr;
	cstring type;
	bool is_from_vector = false;
	if (boolean_exprs.size()!=0 && rand()%2==0) {
		size_t ind = rand()%boolean_exprs.size();
		expr = boolean_exprs.at(ind);
		boolean_exprs.erase(boolean_exprs.begin()+ind);
		is_from_vector = true;
	}
	else {
		int num_trials = 100;
		while (num_trials--) {
			expr = get_operand(rand()%2, &tp, type, false, true);
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
			switch(rand()%2) {
			case 0: expr = new IR::Equ(expr, bool_literal::gen_literal()); break;
			// case 1: expr = new IR::Geq(expr, bool_literal::gen_literal()); break;
			// case 2: expr = new IR::Grt(expr, bool_literal::gen_literal()); break;
			// case 3: expr = new IR::Leq(expr, bool_literal::gen_literal()); break;
			// case 4: expr = new IR::Lss(expr, bool_literal::gen_literal()); break;
			case 1: expr = new IR::Neq(expr, bool_literal::gen_literal()); break;
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

		if (expr != nullptr) {
			boolean_exprs.push_back(expr);
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

IR::Expression* get_compound_operand(const IR::Type** tp, cstring compound_type, bool if_param_in_inc) {
	IR::Expression* expr = nullptr;
	cstring type;
	int num_trials = 100;
	while (num_trials--) {
		expr = expression::get_operand(rand()%2, tp, type, true, if_param_in_inc);
		if (expr!=nullptr && type==compound_type) {
			break;
		}
	}
	return expr;
}

bool expression::get_two_compound_operands(IR::Expression** expr1, IR::Expression** expr2, 
					const IR::Type** tp1, const IR::Type** tp2) {
	bool ret = false;
	int num_trails = 500;
	while (num_trails--) {
		*expr1 = get_compound_operand(tp1, EXPR_TYPE_STRUCT, false);
		*expr2 = get_compound_operand(tp2, EXPR_TYPE_STRUCT, true);
		if (*expr1!=nullptr && *expr2!=nullptr && 
				(*tp1)->is<IR::Type_Struct>() &&
				(*tp2)->is<IR::Type_Struct>()) {
			if ((*tp1)->to<IR::Type_Struct>()->name.name ==
					(*tp2)->to<IR::Type_Struct>()->name.name) {
				ret = true;
				return ret;
			}
		}

		*expr1 = get_compound_operand(tp1, EXPR_TYPE_HEADER, false);
		*expr2 = get_compound_operand(tp2, EXPR_TYPE_HEADER, true);
		if (*expr1!=nullptr && *expr2!=nullptr &&
				(*tp1)->is<IR::Type_Header>() &&
				(*tp2)->is<IR::Type_Header>()) {
			if ((*tp1)->to<IR::Type_Header>()->name.name ==
					(*tp2)->to<IR::Type_Header>()->name.name) {
				ret = true;
				return ret;
			}
		}

		*expr1 = get_compound_operand(tp1, EXPR_TYPE_HUNION, false);
		*expr2 = get_compound_operand(tp2, EXPR_TYPE_HUNION, true);
		if (*expr1!=nullptr && *expr2!=nullptr &&
				(*tp1)->is<IR::Type_HeaderUnion>() &&
				(*tp2)->is<IR::Type_HeaderUnion>()) {
			if ((*tp1)->to<IR::Type_HeaderUnion>()->name.name ==
					(*tp2)->to<IR::Type_HeaderUnion>()->name.name) {
				return true;
				return ret;
			}
		}
	}

	return ret;
}

IR::Expression* expression::construct_compound_cond_expr() {
	IR::Expression* expr = nullptr;
	IR::Expression* expr1, * expr2;
	const IR::Type* tp1;
	const IR::Type* tp2;
	if (get_two_compound_operands(&expr1, &expr2, &tp1, &tp2) == true) {
		switch (rand()%2) {
			case 0: expr = new IR::Neq(expr1, expr2); break;
			case 1: expr = new IR::Equ(expr1, expr2); break;
		}
		if (expr != nullptr) {
			boolean_exprs.push_back(expr);
		}
	}
	return expr;
}

IR::Vector<IR::Argument> *expression::construct_params(std::vector<const IR::Type*> &v_tp) {
	IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();

	for (auto i=v_tp.begin(); i<v_tp.end(); i++) {
		IR::Argument* arg = nullptr;
		const IR::Type* tp = *i;
		IR::Expression* expr = nullptr;
		const IR::Type* expr_tp = nullptr;
		cstring str_expr_tp;
		int num_trials = 400;
		bool expr_found = false;
		while (num_trials--) {
			if (tp->is<IR::Type_Bits>()) {
				auto tp_b = tp->to<IR::Type_Bits>();
				expr = get_operand(rand()%2, &expr_tp, str_expr_tp, rand()%2, false);
				if (expr!=nullptr && 
						mp_expr_2_if_inc_in[expr]==false && 
						str_expr_tp==EXPR_TYPE_BITS) {
					auto expr_tp_b = expr_tp->to<IR::Type_Bits>();
					int size = tp_b->size;
					int expr_size = expr_tp_b->size;
					if (size > expr_size) {
						continue;
					}
					else if (size == expr_size) {
						expr_found = true;
						arg = new IR::Argument(expr);
					}
					else {
						expr_found = true;
						int r_range = rand()%(expr_size-size+1);
						int l_range = r_range+size-1;
						arg = new IR::Argument(new IR::Slice(expr, 
									new IR::Constant(l_range),
									new IR::Constant(r_range)));
					}
				}
			}

			if (expr_found == true) {
				break;
			}
		}

		if (expr_found == true) {
		}

		if (arg != nullptr) {
			args->push_back(arg);
		}
	}

	return args;
}

void expression::construct_list_expr(const IR::Type *tp, 
		IR::Vector<IR::Expression> &exprs, bool *if_contains_stack) {
	if (tp->is<IR::Type_StructLike>()) {
		if (tp->is<IR::Type_HeaderUnion>()) {
			*if_contains_stack = true;
		}
		const IR::Type_StructLike* t = tp->to<IR::Type_StructLike>();
		const IR::StructField* sf = nullptr;
		IR::Vector<IR::Expression> v_exprs;
		for (size_t i=0; i<t->fields.size(); i++) {
			sf = t->fields.at(i);
			construct_list_expr((IR::Type *)sf, v_exprs, if_contains_stack);
		}
		IR::ListExpression *l_expr = new IR::ListExpression(v_exprs);
		exprs.push_back(l_expr);
	}
	else if (tp->is<IR::StructField>()) {
		const IR::StructField* t = tp->to<IR::StructField>();
		construct_list_expr(t->type, exprs, if_contains_stack);
	}
	else if (tp->is<IR::Type_Stack>()) {
		*if_contains_stack = true;
	}
	else if (tp->is<IR::Type_Name>()) {
		auto tp_name = tp->to<IR::Type_Name>();
		construct_list_expr(P4Scope::get_type_by_name(tp_name->path->name.name), 
								exprs, if_contains_stack);
	}
	else if (tp->is<IR::Type_Bits>()) {
		auto expr = new IR::Constant(tp->to<IR::Type_Bits>(), 0);
		exprs.push_back(expr);
	}
	else if (tp->is<IR::Type_Boolean>()) {
		auto expr = new IR::BoolLiteral(false);
		exprs.push_back(expr);
	}
}

} // namespace CODEGEN


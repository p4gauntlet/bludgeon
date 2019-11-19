


#include "expression.h"

namespace CODEGEN {


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
        const IR::Type_Bits* t = tp->to<IR::Type_Bits>();
        type = "type_bits";
        return t;
    }
    else if (tp->is<IR::Type_Boolean>()) {
        const IR::Type_Boolean* t = tp->to<IR::Type_Boolean>();
        type = "type_boolean";
        return t;
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

IR::Expression* expression::get_operand() {
    std::vector<cstring> call_backtrace;
    cstring type;
	const IR::Type* tp = nullptr;

	int num_trials = 100;
	while (num_trials--) {
		if (rand()%2 == 0) {
			tp = pick_lval(P4Scope::name_2_type_param, call_backtrace, type);
		}
		else {
			tp = pick_lval(P4Scope::name_2_type_vars, call_backtrace, type);
		}

		if (tp != nullptr) {
			break;
		}
	}

	if (tp == nullptr) {
		return nullptr;
	}

	std::cout << "call backtrace!!!!!\n";
	for (auto &i : call_backtrace) {
		std::cout << i << std::endl;
	}

	return construct_expr(call_backtrace);
}



} // namespace CODEGEN


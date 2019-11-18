


#include "expression.h"

namespace CODEGEN {

const IR::Type* pick_field(std::map<cstring, const IR::Type*> &mp,
        const IR::Type* tp, std::vector<cstring> &call_bt, cstring &type);

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
}


IR::Expression* expression::gen_cond() {
    std::vector<cstring> call_backtrace;
    cstring type;
    pick_lval(P4Scope::name_2_type_param, call_backtrace, type);

    std::cout << "wdnmlgb\n";
    for (auto &v : P4Scope::name_2_type_param) {
        std::cout << v.first << std::endl;
    }

    std::cout << type << " call_backtrace11111:\n";
    for (auto &v : call_backtrace) {
        std::cout << v << std::endl;
    }

    call_backtrace.clear();

    pick_lval(P4Scope::name_2_type_vars, call_backtrace, type);

    std::cout << type << " call_backtrace22222:\n";
    for (auto &v : call_backtrace) {
        std::cout << v << std::endl;
    }
}



} // namespace CODEGEN


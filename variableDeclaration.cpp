#include "variableDeclaration.h"



namespace CODEGEN {


IR::Declaration_Variable *variableDeclaration::gen() {
    std::vector<int> types = {};
    typeRef *type_ref      = new typeRef(true, types, STRUCT_LIKE);

    type = type_ref->gen();

    IR::Declaration_Variable *ret;
    // Tao: construct list expression
    if (type->is<IR::Type_Bits>()) {
        auto expr = expression2::gen_expr(type);
        ret = new IR::Declaration_Variable(*name, type, expr);
    } else if (type->is<IR::Type_Boolean>()) {
        auto expr = expression2::gen_expr(type);
        ret = new IR::Declaration_Variable(*name, type, expr);
    } else{
        IR::Vector<IR::Expression> exprs;
        bool if_contains_stack = false;
        expression::construct_list_expr(type, exprs, &if_contains_stack);
        bool is_forbidden = false;
        if (type->is<IR::Type_Name>()) {
            auto tp_n = type->to<IR::Type_Name>();
            if (P4Scope::not_initialized_structs.find(tp_n->path->name.name)
                != P4Scope::not_initialized_structs.end()) {
                is_forbidden = true;
            }
        }
        if ((if_contains_stack == false) && (is_forbidden == false)) {
            // ret = new IR::Declaration_Variable(*name, type, new IR::ListExpression(exprs));
            ret = new IR::Declaration_Variable(*name, type, exprs.at(0));
        } else{
            ret = new IR::Declaration_Variable(*name, type);
            // Tao: this needs further initialization
            P4Scope::add_name_2_type_stack(name->name, type);
        }
    }

    //
    P4Scope::add_to_scope((IR::Node *)ret);
    P4Scope::add_lval(type, name->name);
    P4Scope::add_name_2_type_v(name->name, type);

    return ret;
}
} // namespace CODEGEN

#include "variableDeclaration.h"



namespace CODEGEN {
IR::Declaration_Variable *variableDeclaration::gen() {
    std::vector<int> types = {};
    typeRef *type_ref      = new typeRef(true, types, STRUCT_LIKE);
    cstring name           = randstr(6);
    IR::Type *type         = type_ref->gen();

    IR::Declaration_Variable *ret;

    // Tao: construct list expression
    if (type->is<IR::Type_Bits>()) {
        auto expr = expression2::gen_expr(type);
        ret = new IR::Declaration_Variable(name, type, expr);
    } else if (type->is<IR::Type_Boolean>()) {
        auto expr = expression2::gen_expr(type);
        ret = new IR::Declaration_Variable(name, type, expr);
    } else{
        IR::Vector<IR::Expression> exprs;
        bool if_contains_stack = false;
        expression::construct_list_expr(type, exprs, &if_contains_stack);
        bool is_forbidden = false;
        if (type->is<IR::Type_Name>()) {
            auto type_name = type->to<IR::Type_Name>()->path->name.name;
            if (P4Scope::not_initialized_structs.count(type_name) != 0) {
                is_forbidden = true;
            }
        }
        if (not if_contains_stack && not is_forbidden) {
            // ret = new IR::Declaration_Variable(name, type, new IR::ListExpression(exprs));
            ret = new IR::Declaration_Variable(name, type, exprs.at(0));
        } else{
            ret = new IR::Declaration_Variable(name, type);
            // Tao: this needs further initialization
            P4Scope::add_name_2_type_stack(name, type);
        }
    }

    P4Scope::add_to_scope(ret);
    P4Scope::add_lval(type, name, false);
    P4Scope::add_name_2_type_v(name, type);

    return ret;
}
} // namespace CODEGEN

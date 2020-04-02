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
        auto expr = expression::gen_expr(type);
        ret = new IR::Declaration_Variable(name, type, expr);
    } else if (type->is<IR::Type_Boolean>()) {
        auto expr = expression::gen_expr(type);
        ret = new IR::Declaration_Variable(name, type, expr);
    } else if (type->is<IR::Type_Name>()) {
        IR::Vector<IR::Expression> exprs;
        bool is_forbidden = false;
        auto type_name = type->to<IR::Type_Name>()->path->name.name;
        if (P4Scope::not_initialized_structs.count(type_name) != 0) {
            is_forbidden = true;
        }
        // TODO FIX TYPE NAME
        ret = new IR::Declaration_Variable(name, type);
    }
    else {
        BUG("Type %s not supported!", type->node_type_name());
    }


    P4Scope::add_to_scope(ret);
    P4Scope::add_lval(type, name, false);

    return ret;
}
} // namespace CODEGEN

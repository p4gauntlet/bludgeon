#include "variableDeclaration.h"

namespace CODEGEN {

IR::Declaration_Variable *variableDeclaration::gen() {
    std::vector<int> types = {};
    cstring name = randstr(6);
    IR::Type *type = typeRef::gen(true, types, STRUCT_LIKE);

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
        if (not is_forbidden) {
            auto expr = expression::gen_expr(type);
            ret = new IR::Declaration_Variable(name, type, expr);
        } else {
            ret = new IR::Declaration_Variable(name, type);
        }

    } else {
        BUG("Type %s not supported!", type->node_type_name());
    }

    P4Scope::add_to_scope(ret);

    return ret;
}
} // namespace CODEGEN

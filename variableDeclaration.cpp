#include "variableDeclaration.h"

namespace CODEGEN {

IR::Type *gen_type() {
    std::vector<int> percent = {75, 25, 0};
    IR::Type *tp = nullptr;
    switch (randind(percent)) {
    case 0: {
        std::vector<int> b_types = {1}; // only bit<>
        tp = baseType::gen(false, b_types);
        break;
    }
    case 1: {
        auto l_types = P4Scope::get_decls<IR::Type_StructLike>();
        if (l_types.size() == 0) {
            return nullptr;
        }
        auto candidate_type = l_types.at(rand() % l_types.size());
        tp = new IR::Type_Name(candidate_type->name.name);
        break;
    }
    case 2: {
        // tp = headerStackType::gen();
        break;
    }
    }
    return tp;
}

IR::Declaration_Variable *variableDeclaration::gen() {
    cstring name = randstr(6);
    IR::Type *type = gen_type();

    IR::Declaration_Variable *ret = nullptr;

    // Tao: construct list expression
    if (type->is<IR::Type_Bits>()) {
        auto expr = expression::gen_expr(type);
        ret = new IR::Declaration_Variable(name, type, expr);
    } else if (type->is<IR::Type_Boolean>()) {
        auto expr = expression::gen_expr(type);
        ret = new IR::Declaration_Variable(name, type, expr);
    } else if (type->is<IR::Type_Name>()) {
        IR::Vector<IR::Expression> exprs;
        auto type_name = type->to<IR::Type_Name>()->path->name.name;
        // check if struct is forbidden
        if (P4Scope::not_initialized_structs.count(type_name) == 0) {
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

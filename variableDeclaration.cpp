#include "variableDeclaration.h"

#include "common.h"
#include "expression.h"
#include "scope.h"
#include "typeRef.h"

namespace CODEGEN {

IR::Type *gen_type() {
    std::vector<int64_t> percent = {80, 15, 5};
    IR::Type *tp = nullptr;
    switch (randind(percent)) {
    case 0: {
        std::vector<int> b_types = {0, 1};
        tp = baseType::pick_rnd_base_type(b_types);
        break;
    }
    case 1: {
        auto l_types = P4Scope::get_decls<IR::Type_StructLike>();
        if (l_types.size() == 0) {
            return nullptr;
        }
        auto candidate_type = l_types.at(rand() % l_types.size());
        auto type_name = candidate_type->name.name;
        // check if struct is forbidden
        if (P4Scope::not_initialized_structs.count(type_name) == 0) {
            tp = new IR::Type_Name(candidate_type->name.name);
        } else {
            std::vector<int> b_types = {0, 1};
            tp = baseType::pick_rnd_base_type(b_types);
        }
        break;
    }
    case 2: {
        tp = headerStackType::gen();
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
    if (type->is<IR::Type_Bits>() || type->is<IR::Type_InfInt>()) {
        auto expr = expression::gen_expr(type);
        ret = new IR::Declaration_Variable(name, type, expr);
    } else if (type->is<IR::Type_Boolean>()) {
        auto expr = expression::gen_expr(type);
        ret = new IR::Declaration_Variable(name, type, expr);
    } else if (type->is<IR::Type_Name>()) {
        auto expr = expression::gen_expr(type);
        ret = new IR::Declaration_Variable(name, type, expr);
    } else if (type->is<IR::Type_Stack>()) {
        // header stacks do not have an initializer yet
        ret = new IR::Declaration_Variable(name, type);
    } else {
        BUG("Type %s not supported!", type->node_type_name());
    }

    P4Scope::add_to_scope(ret);

    return ret;
}
} // namespace CODEGEN

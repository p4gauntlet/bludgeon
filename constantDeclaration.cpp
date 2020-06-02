#include "constantDeclaration.h"

#include "common.h"
#include "expression.h"
#include "scope.h"
#include "typeRef.h"

namespace CODEGEN {

IR::Type *gen_constant_type() {
    std::vector<int64_t> percent = {80, 20};
    IR::Type *tp = nullptr;
    switch (randind(percent)) {
    case 0: {
        std::vector<int> b_types = {0, 1};
        tp = baseType::pick_rnd_base_type(b_types);
        break;
    }
    case 1: {
        tp = new IR::Type_InfInt();
        break;
    }
    }
    return tp;
}

IR::Declaration_Constant *constantDeclaration::gen() {
    cstring name = randstr(6);
    IR::Type *type = gen_constant_type();

    IR::Declaration_Constant *ret = nullptr;
    // constant declarations need to be compile-time known
    P4Scope::req.compile_time_known = true;

    if (type->is<IR::Type_Bits>() || type->is<IR::Type_InfInt>() ||
        type->is<IR::Type_Boolean>()) {
        auto expr = expression::gen_expr(type);
        ret = new IR::Declaration_Constant(name, type, expr);
    } else {
        BUG("Type %s not supported!", type->node_type_name());
    }

    P4Scope::add_to_scope(ret);

    return ret;
}

} // namespace CODEGEN

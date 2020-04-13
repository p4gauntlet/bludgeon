

#include "constantDeclaration.h"

namespace CODEGEN {
IR::Declaration_Constant *gen() {
    IR::Expression *expr;
    std::vector<int> b_types = {0, 1};
    IR::Type *tp = baseType::pick_rnd_base_type(b_types);
    cstring name = randstr(4);

    if (tp->is<IR::Type_Boolean>()) {
        expr = new IR::BoolLiteral(false);
    } else {
        expr = new IR::Constant(tp->to<IR::Type_Bits>(), rand() % 8 + 1);
    }

    // add to the scope
    auto ret = new IR::Declaration_Constant(name, tp, expr);
    P4Scope::add_to_scope(ret);
    return ret;
}

} // namespace CODEGEN

#include "constantDeclaration.h"

#include "common.h"
#include "expression.h"
#include "scope.h"
#include "typeRef.h"

namespace CODEGEN {

IR::Type *gen_constant_type() {
    IR::Type *tp = nullptr;
    std::vector<int64_t> type_probs = {
        PCT.CONSTANTDECLARATION_BASETYPE_BOOL,
        PCT.CONSTANTDECLARATION_BASETYPE_ERROR,
        PCT.CONSTANTDECLARATION_BASETYPE_INT,
        PCT.CONSTANTDECLARATION_BASETYPE_STRING,
        PCT.CONSTANTDECLARATION_BASETYPE_BIT,
        PCT.CONSTANTDECLARATION_BASETYPE_SIGNED_BIT,
        PCT.CONSTANTDECLARATION_BASETYPE_VARBIT};
    tp = baseType::pick_rnd_base_type(type_probs);
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
    P4Scope::req.compile_time_known = false;

    P4Scope::add_to_scope(ret);

    return ret;
}

} // namespace CODEGEN

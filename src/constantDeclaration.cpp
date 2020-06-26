#include "constantDeclaration.h"

#include "baseType.h"
#include "common.h"
#include "expression.h"
#include "scope.h"
#include "typeRef.h"

namespace CODEGEN {

IR::Declaration_Constant *constantDeclaration::gen() {
    cstring name = randstr(6);
    typeref_probs type_percent = {
        PCT.CONSTANTDECLARATION_BASETYPE_BIT,
        PCT.CONSTANTDECLARATION_BASETYPE_SIGNED_BIT,
        PCT.CONSTANTDECLARATION_BASETYPE_VARBIT,
        PCT.CONSTANTDECLARATION_BASETYPE_INT,
        PCT.CONSTANTDECLARATION_BASETYPE_ERROR,
        PCT.CONSTANTDECLARATION_BASETYPE_BOOL,
        PCT.CONSTANTDECLARATION_BASETYPE_STRING,
        PCT.CONSTANTDECLARATION_DERIVED_ENUM,
        PCT.CONSTANTDECLARATION_DERIVED_HEADER,
        PCT.CONSTANTDECLARATION_DERIVED_HEADER_STACK,
        PCT.CONSTANTDECLARATION_DERIVED_STRUCT,
        PCT.CONSTANTDECLARATION_DERIVED_HEADER_UNION,
        PCT.CONSTANTDECLARATION_DERIVED_TUPLE,
        PCT.CONSTANTDECLARATION_TYPE_VOID,
        PCT.CONSTANTDECLARATION_TYPE_MATCH_KIND,
    };

    IR::Type *tp = typeRef::pick_rnd_type(type_percent);

    IR::Declaration_Constant *ret = nullptr;
    // constant declarations need to be compile-time known
    P4Scope::req.compile_time_known = true;

    if (tp->is<IR::Type_Bits>() || tp->is<IR::Type_InfInt>() ||
        tp->is<IR::Type_Boolean>() || tp->is<IR::Type_Name>()) {
        auto expr = expression::gen_expr(tp);
        ret = new IR::Declaration_Constant(name, tp, expr);
    } else {
        BUG("Type %s not supported!", tp->node_type_name());
    }
    P4Scope::req.compile_time_known = false;

    P4Scope::add_to_scope(ret);

    return ret;
}

} // namespace CODEGEN

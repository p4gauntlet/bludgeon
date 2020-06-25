#include "variableDeclaration.h"

#include "common.h"
#include "expression.h"
#include "scope.h"
#include "typeRef.h"
#include "baseType.h"
#include "headerStackType.h"

namespace CODEGEN {

IR::Type *gen_type() {
    std::vector<int64_t> percent = {PCT.VARIABLEDECLARATION_TYPE_BASE,
                                    PCT.VARIABLEDECLARATION_TYPE_STRUCT,
                                    PCT.VARIABLEDECLARATION_TYPE_STACK};
    IR::Type *tp = nullptr;
    std::vector<int64_t> type_probs = {
        PCT.VARIABLEDECLARATION_BASETYPE_BOOL,
        PCT.VARIABLEDECLARATION_BASETYPE_ERROR,
        PCT.VARIABLEDECLARATION_BASETYPE_INT,
        PCT.VARIABLEDECLARATION_BASETYPE_STRING,
        PCT.VARIABLEDECLARATION_BASETYPE_BIT,
        PCT.VARIABLEDECLARATION_BASETYPE_SIGNED_BIT,
        PCT.VARIABLEDECLARATION_BASETYPE_VARBIT};
    switch (randind(percent)) {
    case 0: {
        tp = baseType::pick_rnd_base_type(type_probs);
        break;
    }
    case 1: {
        auto l_types = P4Scope::get_decls<IR::Type_StructLike>();
        if (l_types.size() == 0) {
            return nullptr;
        }
        auto candidate_type = l_types.at(get_rnd_int(0, l_types.size() - 1));
        auto type_name = candidate_type->name.name;
        // check if struct is forbidden
        if (P4Scope::not_initialized_structs.count(type_name) == 0) {
            tp = new IR::Type_Name(candidate_type->name.name);
        } else {
            tp = baseType::pick_rnd_base_type(type_probs);
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

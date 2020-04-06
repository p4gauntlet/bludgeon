#include "headerStackType.h"

namespace CODEGEN {
IR::Type *headerStackType::gen() {
    IR::Type *tp      = type_name->gen();
    IR::Expression *e = bit_literal::gen_int();

    if ((tp == nullptr) || (e == nullptr)) {
        return nullptr;
    }
    auto tp_name = tp->to<IR::Type_Name>();

    // Tao: STRUCT_LIKE is used to generate Declaration_Variable
    // should not be a struct stack
    if (for_type == STRUCT_LIKE) {
        auto typed_name = P4Scope::get_type_by_name(
            tp_name->path->name.name);
        if (typed_name->is<IR::Type_Struct>()) {
            return nullptr;
        }
    }

    IR::Type *ret = new IR::Type_Stack(tp, e);
    return ret;
}
} // namespace CODEGEN

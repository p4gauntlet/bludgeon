

#include "typedefDeclaration.h"

namespace CODEGEN {

void typedefDeclaration::gen_t() {
    const IR::Type *type = nullptr;
    while (1) {
        type = typeRef::gen();
        if (type != nullptr) {
            break;
        }
    }
}

void typedefDeclaration::gen_base_t() {
}

IR::Type_Typedef *typedefDeclaration::gen_typedef() {
    cstring name = randstr(5);
    IR::Type *type = nullptr;

    gen_t();
    auto ret = new IR::Type_Typedef(name, type);
    P4Scope::add_to_scope(ret);
    return ret;
}

IR::Type_Newtype *typedefDeclaration::gen_newtype() {
    cstring name = randstr(5);
    IR::Type *type = nullptr;

    gen_base_t();
    auto ret = new IR::Type_Newtype(name, type);
    P4Scope::add_to_scope(ret);
    return ret;
}

IR::Type *typedefDeclaration::gen() {
    // Tao: we only have typedef now, no newtype
    return gen_typedef();
}

} // namespace CODEGEN

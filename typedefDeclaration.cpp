

#include "typedefDeclaration.h"

namespace CODEGEN {

void typedefDeclaration::gen_t() {
    const IR::Type *type = nullptr;
    while (1) {
        if (rand() % 2 == 0) {
            std::vector<int> types;
            type = typeRef::gen(true, types, "");
        } else {
            auto struct_like_decls = P4Scope::get_decls<IR::Type_StructLike>();
        }

        if (type != nullptr) {
            break;
        }
    }
}

void typedefDeclaration::gen_base_t() {
    IR::Type *type = nullptr;

    std::vector<int> b_types;
    auto base_type = new baseType(true, b_types);
    type = base_type->gen();
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

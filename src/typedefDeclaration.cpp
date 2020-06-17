

#include "typedefDeclaration.h"

namespace CODEGEN {

IR::Type *gen_t() {
    std::vector<int64_t> percent = {PCT.TYPEDEFDECLARATION_BASE,
                                    PCT.TYPEDEFDECLARATION_STRUCTLIKE,
                                    PCT.TYPEDEFDECLARATION_STACK};
    IR::Type *tp = nullptr;
    switch (randind(percent)) {
    case 0: {
        std::vector<int> b_types = {1}; // only bit<>
        tp = baseType::pick_rnd_base_type(b_types);
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

void typedefDeclaration::gen_base_t() {}

IR::Type_Typedef *typedefDeclaration::gen_typedef() {
    cstring name = randstr(5);
    IR::Type *type = gen_t();
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

#include "typeRef.h"

namespace CODEGEN {

IR::Type *typeRef::gen() {
    IR::Type *tp = nullptr;
    std::vector<int> percent = {75, 25, 0};

    switch (randind(percent)) {
    case 0: {
        std::vector<int> b_types = {1}; // only bit<>
        tp = baseType::gen(false, b_types);
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

} // namespace CODEGEN

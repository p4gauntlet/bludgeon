

#include "structField.h"

namespace CODEGEN {
IR::StructField *structField::gen(cstring for_type) {
    cstring name = randstr(4);

    std::vector<int> type;
    IR::Type *tp = typeRef::gen(true, type, for_type);

    if (tp == nullptr) {
        return nullptr;
    }

    return new IR::StructField(name, tp);
}

} // namespace CODEGEN

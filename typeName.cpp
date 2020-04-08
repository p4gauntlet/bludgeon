

#include "typeName.h"

namespace CODEGEN {
IR::Type *typeName::gen(cstring for_type) {
    return prefixedType::gen(for_type);
}

} // namespace CODEGEN

#ifndef _PREFIXEDTYPE_H_
#define _PREFIXEDTYPE_H_

#include <vector>

#include "ir/ir.h"

#include "common.h"
#include "scope.h"

namespace CODEGEN {
class prefixedType {
 public:
    const char *types[2] = {
        "TYPE_IDENTIFIER",
        ". TYPE_IDENTIFIER" // not supported
    };

    prefixedType() {}

    static IR::Type *gen();
};
} // namespace CODEGEN

#endif

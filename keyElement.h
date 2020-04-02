#ifndef _KEYELEMENT_H_
#define _KEYELEMENT_H_

#include "ir/ir.h"

#include "annotations.h"
#include "scope.h"
#include "codegen.h"

namespace CODEGEN {
class keyElement {
public:
    keyElement() {
    }

    ~keyElement() {
    }

    static IR::KeyElement *gen(cstring match_kind);
};
} // namespace CODEGEN



#endif

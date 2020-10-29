#ifndef _IDENTIFIERLIST_H_
#define _IDENTIFIERLIST_H_

#include "ir/ir.h"

namespace CODEGEN {

class identifierList {
 public:
    const char *types[1] = {"name"};

    identifierList() {}

    static IR::IndexedVector<IR::Declaration_ID> gen(size_t len);
};

} // namespace CODEGEN

#endif

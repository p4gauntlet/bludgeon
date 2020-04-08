#ifndef _TYPEREF_H_
#define _TYPEREF_H_

#include "ir/ir.h"

#include "baseType.h"
#include "headerStackType.h"
#include "typeName.h"

#include "common.h"

namespace CODEGEN {
class typeRef {
  public:
    const char *types[5] = {
        "baseType", "typeName",
        "specializedType", // not
        "headerStackType",
        "tupleType" // not
    };


    typeRef() {}

    static IR::Type *gen(bool if_rand, std::vector<int> &type,
                         cstring for_type);
};
} // namespace CODEGEN

#endif

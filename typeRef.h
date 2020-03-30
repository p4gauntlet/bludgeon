#ifndef _TYPEREF_H_
#define _TYPEREF_H_

#include "ir/ir.h"

#include "baseType.h"
#include "typeName.h"
#include "headerStackType.h"

#include "common.h"


namespace CODEGEN {
class typeRef {
public:
    const char *types[5] = {
        "baseType",
        "typeName",
        "specializedType",  // not
        "headerStackType",
        "tupleType"         // not
    };

    bool if_random;
    std::vector<int> type;
    cstring for_type;

    typeRef(bool if_rand, std::vector<int>& type, cstring for_type) :
        if_random(if_rand),
        type(type),
        for_type(for_type) {
    }

    IR::Type *gen();
};
} // namespace CODEGEN



#endif

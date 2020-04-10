#ifndef _SPECIFIEDIDENTIFIERLIST_H_
#define _SPECIFIEDIDENTIFIERLIST_H_

#include "ir/ir.h"

#include "codegen.h"
#include "expression.h"

namespace CODEGEN {
class specifiedIdentifierList {
  public:
    const char *types[1] = {"specifiedIdentifier"};

    specifiedIdentifierList() {}

    // TODO: Tao: better expr generation
    static IR::IndexedVector<IR::SerEnumMember> gen(size_t len);
};
} // namespace CODEGEN

#endif

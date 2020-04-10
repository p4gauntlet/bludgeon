#ifndef _SPECIFIEDIDENTIFIER_H_
#define _SPECIFIEDIDENTIFIER_H_

#include "ir/ir.h"

#include "codegen.h"
#include "expression.h"

namespace CODEGEN {
class specifiedIdentifier {
  public:
    const char *types[0] = {};

    specifiedIdentifier() {}

    // TODO: Tao: better expr generation
    static IR::IndexedVector<IR::SerEnumMember> gen(size_t len);
};
} // namespace CODEGEN

#endif

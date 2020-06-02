#ifndef _KEYELEMENTLIST_H_
#define _KEYELEMENTLIST_H_

#include "ir/ir.h"

#include "keyElement.h"

namespace CODEGEN {
class keyElementList {
  public:
    keyElementList() {}

    ~keyElementList() {}

    static IR::Key *gen(size_t len);
};
} // namespace CODEGEN

#endif

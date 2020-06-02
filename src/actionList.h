#ifndef _ACTIONLIST_H_
#define _ACTIONLIST_H_

#include "ir/ir.h"


namespace CODEGEN {
class actionList {
  public:
    const char *types[0] = {};

    actionList() {}

    ~actionList() {}

    static IR::ActionList *gen(size_t len);
};
} // namespace CODEGEN

#endif

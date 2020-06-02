#ifndef _TABLEPROPERTYLIST_H_
#define _TABLEPROPERTYLIST_H_

#include "ir/ir.h"

#include "tableProperty.h"

namespace CODEGEN {
class tablePropertyList {
  public:
    const char *types[1] = {"tableProperty"};

    tablePropertyList() {}

    static IR::TableProperties *gen();
};
} // namespace CODEGEN

#endif

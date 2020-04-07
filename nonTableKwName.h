#ifndef _NONTABLEKWNAME_H_
#define _NONTABLEKWNAME_H_

#include "ir/ir.h"

namespace CODEGEN {

class nonTableKwName {
  public:
    const char *types[5] = {"IDENTIFIER", "TYPE_IDENTIFIER", "APPLY", "STATE",
                            "TYPE"};

    nonTableKwName() {}
};

} // namespace CODEGEN

#endif
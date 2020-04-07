#ifndef _BASETYPE_H_
#define _BASETYPE_H_

#include "ir/ir.h"

#include "bit.h"
#include "bool.h"
#include "int.h"

namespace CODEGEN {
class baseType {
  public:
    const char *types[10] = {
        "BOOL",
        "ERROR", // not
        "INT",
        "BIT",
        "BIT <INTEGER>",
        "INT <INTEGER>",
        "VARBIT <INTEGER>",       // not
        "BIT < (expression) >",   // not
        "INT < (expression) >",   // not
        "VARBIT < (expression) >" // not
    };

    bool if_random;
    std::vector<int> type;

    baseType(bool if_rand, std::vector<int> &type)
        : if_random(if_rand), type(type) {}

    IR::Type *gen();
};
} // namespace CODEGEN

#endif

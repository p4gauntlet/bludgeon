#ifndef _BLOCKSTATEMENT_H_
#define _BLOCKSTATEMENT_H_

#include "ir/ir.h"

#include "common.h"

// minimum and maximum number of statements
#define MAX_STAT 10
#define MIN_STAT 3

namespace CODEGEN {
class blockStatement {
  public:
    blockStatement() {}
    ~blockStatement() {}

    static IR::BlockStatement *gen(bool if_in_func = false);
};
} // namespace CODEGEN

#endif // ifndef _BLOCKSTATEMENT_H_

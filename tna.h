#ifndef _TNA_H_
#define _TNA_H_

#include "ir/ir.h"

// minimum and maximum number of statements
#define MIN_T_DECLS 1
#define MAX_T_DECLS 8

namespace CODEGEN {
class TNA {
  public:
    TNA() {}
    ~TNA() {}

    static void generate_includes(std::ostream *ostream);
    static IR::P4Program *gen();
};
} // namespace CODEGEN

#endif // ifndef _TNA_H_

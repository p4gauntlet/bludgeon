#ifndef _V1MODEL_H_
#define _V1MODEL_H_

#include "ir/ir.h"

// minimum and maximum number of statements
#define MIN_T_DECLS 1
#define MAX_T_DECLS 8

namespace CODEGEN {
class V1Model {
  public:
    V1Model() {}
    ~V1Model() {}

    static void generate_includes(std::ostream *ostream);
    static IR::P4Program *gen();
};
} // namespace CODEGEN

#endif // ifndef _V1MODEL_H_

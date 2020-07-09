#ifndef _V1MODEL_H_
#define _V1MODEL_H_

#include "ir/ir.h"

namespace CODEGEN {
class V1Model {
  public:
    V1Model() {}
    ~V1Model() {}

    static void generate_includes(std::ostream *ostream);
    static IR::P4Program *gen();

  private:
    static void set_probabilities();
};
} // namespace CODEGEN

#endif // ifndef _V1MODEL_H_

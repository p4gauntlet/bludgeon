#ifndef _TOP_H_
#define _TOP_H_

#include "ir/ir.h"

namespace CODEGEN {
class Top {
  public:
    Top() {}
    ~Top() {}

    static void generate_includes(std::ostream *ostream);
    static IR::P4Program *gen();
};
} // namespace CODEGEN

#endif // ifndef _TOP_H_

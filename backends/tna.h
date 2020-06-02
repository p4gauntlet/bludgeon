#ifndef _TNA_H_
#define _TNA_H_

#include "ir/ir.h"
#include "scope.h"

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

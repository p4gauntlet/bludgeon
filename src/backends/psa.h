#ifndef _PSA_H_
#define _PSA_H_

#include "ir/ir.h"
#include "scope.h"

namespace CODEGEN {

class PSA {

  public:
    PSA() {}
    ~PSA() {}

    static void generate_includes(std::ostream *ostream);
    static IR::P4Program *gen();
};
} // namespace CODEGEN

#endif // ifndef _PSA_H_

#ifndef _EXTERNDECLARATION_H_
#define _EXTERNDECLARATION_H_

#include "blockStatement.h"
#include "ir/ir.h"
#include "parameterList.h"

namespace CODEGEN {
class externDeclaration {
  public:
    const char *types[0] = {};

    externDeclaration() {}

    ~externDeclaration() {}

    static IR::Method *gen();

  protected:
    static IR::Type *gen_return_type();
};
} // namespace CODEGEN

#endif // ifndef _EXTERNDECLARATION_H_

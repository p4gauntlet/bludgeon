#ifndef _ARGUMENT_H_
#define _ARGUMENT_H_

#include "ir/ir.h"

namespace CODEGEN {
class argument {
  public:
    const char *types[2] = {"expression", "DONTCARE"};

    argument() {}
    static IR::Expression *gen_input_arg(const IR::Parameter *param);
    static bool check_input_arg(const IR::Parameter *param);
};
} // namespace CODEGEN

#endif

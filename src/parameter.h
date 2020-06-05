#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#include "ir/ir.h"

namespace CODEGEN {
class parameter {
  public:
    const char *types[0] = {};

    parameter() {}
    ~parameter() {}

    static IR::Parameter *gen(bool if_none_dir = false);
    static IR::Parameter *gen_param(IR::Direction dir, cstring p_name,
                                    cstring t_name);
};
} // namespace CODEGEN

#endif // ifndef _PARAMETER_H_

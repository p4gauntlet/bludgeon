#ifndef _PARAMETERLIST_H_
#define _PARAMETERLIST_H_

#include "ir/ir.h"

#include "expression.h"
#include "parameter.h"
#include "scope.h"

namespace CODEGEN {
class parameterList {
  public:
    const char *types[1] = {"nonEmptyParameterList"};

    parameterList() {}

    static IR::ParameterList *gen_tf_ing_params();
    static IR::ParameterList *gen_ing_params();
    static IR::ParameterList *gen(bool if_none_dir = false);
    static IR::ParameterList *gen_par_params();
};
} // namespace CODEGEN

#endif // ifndef _PARAMETERLIST_H_

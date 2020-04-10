#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#include "ir/ir.h"

#include "baseType.h"
#include "codegen.h"

namespace CODEGEN {
class parameter {
  public:
    const char *types[0] = {};

    parameter() {}
    ~parameter() {}

    // Tao: the parameter seems can only be int<> bit<> in bmv2 model
    static IR::Parameter *gen(bool if_none_dir = false);
    static IR::Parameter *gen_ing_headers();
    static IR::Parameter *gen_ing_meta();
    static IR::Parameter *gen_ing_sm();
    static IR::Parameter *gen_pkt_in();
    static IR::Parameter *gen_out_header();
    static IR::Parameter *gen_param(IR::Direction dir, cstring p_name,
                                    cstring t_name);
};
} // namespace CODEGEN

#endif // ifndef _PARAMETER_H_

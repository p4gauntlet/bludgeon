#ifndef _BASETYPE_H_
#define _BASETYPE_H_

#include "ir/ir.h"
#include "common.h"

namespace CODEGEN {

class baseType {
  public:
    const char *types[10] = {
        "BOOL",
        "ERROR", // not
        "INT",
        "BIT",
        "BIT <INTEGER>",
        "INT <INTEGER>",
        "VARBIT <INTEGER>",       // not
        "BIT < (expression) >",   // not
        "INT < (expression) >",   // not
        "VARBIT < (expression) >" // not
    };

    baseType() {}
    static constexpr int bit_widths[5] = {8, 16, 32, 64, 128};

    static IR::Type_Base *pick_rnd_base_type(std::vector<int> &type);

    static IR::Type_Boolean *gen_bool_type() { return new IR::Type_Boolean(); }
    static IR::Type_InfInt *gen_int_type() { return new IR::Type_InfInt(); }
    static IR::Type_Bits *gen_bit_type(bool isSigned);

    static IR::BoolLiteral *gen_bool_literal();
    static IR::Constant *gen_int_literal(size_t bit_width = INTEGER_WIDTH,
                                         bool not_zero = false);
    static IR::Constant *gen_bit_literal(const IR::Type *tb,
                                         bool not_zero = false);
};
} // namespace CODEGEN

#endif

#ifndef _P4PARSER_H_
#define _P4PARSER_H_

#include "ir/ir.h"

#include "common.h"
#include "p4state.h"
#include "parameterList.h"
#include "scope.h"

namespace CODEGEN {
class p4Parser {
  public:

    p4Parser() {}

    ~p4Parser() {}

    static IR::P4Parser *gen_sys_p();
    static IR::P4Parser *gen_tofino_p();
};
} // namespace CODEGEN

#endif // ifndef _P4PARSER_H_

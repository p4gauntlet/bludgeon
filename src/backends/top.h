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

  private:
    static IR::P4Parser *gen_p();
    static IR::P4Control *gen_egress();
    static IR::P4Control *gen_ingress();
    static IR::Type_Parser *gen_parser_type();
    static IR::Type_Control *gen_ingress_type();
    static IR::Type_Package *gen_package();
    static IR::Declaration_Instance *gen_main();
    static IR::MethodCallStatement *gen_deparser_emit_call();
};
} // namespace CODEGEN

#endif // ifndef _TOP_H_



#include "functionDeclaration.h"

namespace CODEGEN {

IR::Function *functionDeclaration::gen() {
    cstring name = randstr(7);
    IR::Type_Method *tm = nullptr;
    IR::BlockStatement *blk = nullptr;
    P4Scope::start_local_scope();
    IR::ParameterList *params = parameterList::gen(false);

    IR::Type_Bits *r_tp = new IR::Type_Bits(2 << (rand() % 4), false);
    tm = new IR::Type_Method(r_tp, params);

    P4Scope::ret_type = r_tp;
    blk = blockStatement::gen(true);
    P4Scope::ret_type = nullptr;

    auto ret = new IR::Function(name, tm, blk);
    P4Scope::end_local_scope();
    P4Scope::add_to_scope(ret);
    return ret;
}


} // namespace CODEGEN

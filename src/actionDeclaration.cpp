

#include "actionDeclaration.h"

namespace CODEGEN {
IR::P4Action *actionDeclaration::gen() {
    cstring name = randstr(5);
    IR::ParameterList *params = nullptr;
    IR::BlockStatement *blk = nullptr;
    P4Scope::start_local_scope();
    P4Scope::prop.in_action = true;
    params = parameterList::gen();

    blk = blockStatement::gen();

    auto ret = new IR::P4Action(name, params, blk);

    P4Scope::prop.in_action = false;
    P4Scope::end_local_scope();

    P4Scope::add_to_scope(ret);

    return ret;
}

} // namespace CODEGEN

#include "externDeclaration.h"

#include "baseType.h"
#include "expression.h"
#include "parameterList.h"
#include "functionDeclaration.h"
#include "scope.h"

namespace CODEGEN {


IR::Method *externDeclaration::gen() {
    cstring name = randstr(7);
    IR::Type_Method *tm = nullptr;
    P4Scope::start_local_scope();
    IR::ParameterList *params = parameterList::gen();

    IR::Type *r_tp = functionDeclaration::gen_return_type();
    tm = new IR::Type_Method(r_tp, params);
    auto ret = new IR::Method(name, tm);
    P4Scope::end_local_scope();
    P4Scope::add_to_scope(ret);
    return ret;
}

} // namespace CODEGEN

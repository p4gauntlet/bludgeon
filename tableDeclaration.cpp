#include "tableDeclaration.h"

#include "common.h"
#include "scope.h"

namespace CODEGEN {
IR::P4Table *tableDeclaration::gen() {
    IR::TableProperties *tb_properties = tablePropertyList::gen();
    cstring name = randstr(6);
    auto ret = new IR::P4Table(name, tb_properties);
    P4Scope::add_to_scope(ret);
    P4Scope::callable_tables.emplace(ret);
    return ret;
}
} // namespace CODEGEN

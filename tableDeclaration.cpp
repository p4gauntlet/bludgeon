#include "tableDeclaration.h"


namespace CODEGEN {
IR::P4Table *tableDeclaration::gen() {
    auto tab_property_list_gen = new tablePropertyList();
    tb_properties = tab_property_list_gen->gen();
    auto ret = new IR::P4Table(*name, tb_properties);
    P4Scope::add_to_scope(ret);
    P4Scope::callable_tables.emplace(ret);
    return ret;
}
} // namespace CODEGEN

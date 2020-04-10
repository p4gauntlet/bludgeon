

#include "tablePropertyList.h"

namespace CODEGEN {

IR::TableProperties *tablePropertyList::gen() {
    IR::IndexedVector<IR::Property> tab_properties;

    auto tab_property_gen = new tableProperty();

    tab_properties.push_back(tab_property_gen->gen_keys());
    tab_properties.push_back(tab_property_gen->gen_act_lists());

    return new IR::TableProperties(tab_properties);
}

} // namespace CODEGEN

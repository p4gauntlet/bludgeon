#ifndef _TABLEPROPERTYLIST_H_
#define _TABLEPROPERTYLIST_H_

#include "ir/ir.h"


#include "tableProperty.h"

namespace CODEGEN {
class tablePropertyList {
public:
    const char *types[1] = {
        "tableProperty"
    };


    // Keys
    // Actions
    //



    tablePropertyList() {
    }

    IR::TableProperties *gen() {
        IR::IndexedVector<IR::Property> tab_properties;

        auto tab_property_gen = new tableProperty();

        tab_properties.push_back(tab_property_gen->gen_keys());
        tab_properties.push_back(tab_property_gen->gen_act_lists());

        return new IR::TableProperties(tab_properties);
    }
};
} // namespace CODEGEN



#endif

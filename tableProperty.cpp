#include "tableProperty.h"

namespace CODEGEN {
IR::Property *tableProperty::gen_keys() {
    cstring name = IR::TableProperties::keyPropertyName;
    auto keys    = keyElementList::gen(rand() % 4);

    // isConstant --> false
    return new IR::Property(name, keys, false);
}


IR::Property *tableProperty::gen_act_lists() {
    IR::ID *name = new IR::ID(
        IR::TableProperties::actionsPropertyName);
    auto actlist_gen = new actionList();
    auto acts        = actlist_gen->gen(rand() % 4);

    return new IR::Property(*name, acts, false);
}
} // namespace CODEGEN

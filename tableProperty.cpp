#include "tableProperty.h"
#include "actionList.h"
#include "keyElementList.h"

namespace CODEGEN {
IR::Property *tableProperty::gen_keys() {
    cstring name = IR::TableProperties::keyPropertyName;
    auto keys = keyElementList::gen(rand() % 4);

    // isConstant --> false
    return new IR::Property(name, keys, false);
}

IR::Property *tableProperty::gen_act_lists() {
    cstring name = IR::TableProperties::actionsPropertyName;
    auto acts = actionList::gen(rand() % 4);

    return new IR::Property(name, acts, false);
}
} // namespace CODEGEN

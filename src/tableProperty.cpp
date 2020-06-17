#include "tableProperty.h"
#include "actionList.h"
#include "keyElementList.h"

namespace CODEGEN {
IR::Property *tableProperty::gen_keys() {
    cstring name = IR::TableProperties::keyPropertyName;
    auto keys = keyElementList::gen(get_rnd_int(0, 3));

    // isConstant --> false
    return new IR::Property(name, keys, false);
}

IR::Property *tableProperty::gen_act_lists() {
    cstring name = IR::TableProperties::actionsPropertyName;
    auto acts = actionList::gen(get_rnd_int(0, 3));

    return new IR::Property(name, acts, false);
}
} // namespace CODEGEN

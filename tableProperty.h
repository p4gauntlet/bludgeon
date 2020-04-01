#ifndef _TABLEPROPERTY_H_
#define _TABLEPROPERTY_H_

#include "ir/ir.h"


#include "codegen.h"

namespace CODEGEN {
class tableProperty {
public:

    tableProperty() {}

    // Tao: note that we always tag the prperty as not const
    static IR::Property *gen_keys();
    static IR::Property *gen_act_lists();

};
} // namespace CODEGEN


#endif // ifndef _TABLEPROPERTY_H_

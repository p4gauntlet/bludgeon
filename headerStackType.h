#ifndef _HEADERSTACKTYPE_H_
#define _HEADERSTACKTYPE_H_

#include "ir/ir.h"


#include "typeName.h"
#include "expression.h"

#include "scope.h"

namespace CODEGEN {
class headerStackType {
public:
    const char *types[0] = {
    };

    cstring for_type;


    typeName *type_name;


    headerStackType(cstring for_type) : for_type(for_type) {
        type_name = new typeName(for_type);
    }

    ~headerStackType() {
        delete type_name;
    }

    IR::Type *gen();
};
}  // namespace CODEGEN



#endif

#include "parameter.h"

#include "baseType.h"
#include "common.h"
#include "scope.h"
#include "typeRef.h"

namespace CODEGEN {

IR::Parameter *parameter::gen(bool if_none_dir) {
    cstring name = randstr(4);
    IR::Type *tp;
    IR::Direction dir;
    typeref_probs type_percent;

    if (if_none_dir) {
        type_percent = {
            PCT.PARAMETER_NONEDIR_BASETYPE_BIT,
            PCT.PARAMETER_NONEDIR_BASETYPE_SIGNED_BIT,
            PCT.PARAMETER_NONEDIR_BASETYPE_VARBIT,
            PCT.PARAMETER_NONEDIR_BASETYPE_INT,
            PCT.PARAMETER_NONEDIR_BASETYPE_ERROR,
            PCT.PARAMETER_NONEDIR_BASETYPE_BOOL,
            PCT.PARAMETER_NONEDIR_BASETYPE_STRING,
            PCT.PARAMETER_NONEDIR_DERIVED_ENUM,
            PCT.PARAMETER_NONEDIR_DERIVED_HEADER,
            PCT.PARAMETER_NONEDIR_DERIVED_HEADER_STACK,
            PCT.PARAMETER_NONEDIR_DERIVED_STRUCT,
            PCT.PARAMETER_NONEDIR_DERIVED_HEADER_UNION,
            PCT.PARAMETER_NONEDIR_DERIVED_TUPLE,
            PCT.PARAMETER_NONEDIR_TYPE_VOID,
            PCT.PARAMETER_NONEDIR_TYPE_MATCH_KIND,
        };
        dir = IR::Direction::None;
    } else {
        type_percent = {
            PCT.PARAMETER_BASETYPE_BIT,    PCT.PARAMETER_BASETYPE_SIGNED_BIT,
            PCT.PARAMETER_BASETYPE_VARBIT, PCT.PARAMETER_BASETYPE_INT,
            PCT.PARAMETER_BASETYPE_ERROR,  PCT.PARAMETER_BASETYPE_BOOL,
            PCT.PARAMETER_BASETYPE_STRING, PCT.PARAMETER_DERIVED_ENUM,
            PCT.PARAMETER_DERIVED_HEADER,  PCT.PARAMETER_DERIVED_HEADER_STACK,
            PCT.PARAMETER_DERIVED_STRUCT,  PCT.PARAMETER_DERIVED_HEADER_UNION,
            PCT.PARAMETER_DERIVED_TUPLE,   PCT.PARAMETER_TYPE_VOID,
            PCT.PARAMETER_TYPE_MATCH_KIND,
        };
        std::vector<int64_t> dir_percent = {PCT.PARAMETER_DIR_IN,
                                            PCT.PARAMETER_DIR_OUT,
                                            PCT.PARAMETER_DIR_INOUT};
        switch (randind(dir_percent)) {
        case 0:
            dir = IR::Direction::In;
            break;
        case 1:
            dir = IR::Direction::Out;
            break;
        case 2:
            dir = IR::Direction::InOut;
            break;
        }
    }
    tp = typeRef::pick_rnd_type(type_percent);

    return new IR::Parameter(name, dir, tp);
}

IR::Parameter *parameter::gen_param(IR::Direction dir, cstring p_name,
                                    cstring t_name) {
    IR::Type *tp = new IR::Type_Name(new IR::Path(t_name));
    return new IR::Parameter(p_name, dir, tp);
}

} // namespace CODEGEN

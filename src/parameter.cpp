#include "parameter.h"

#include "baseType.h"
#include "common.h"
#include "scope.h"

namespace CODEGEN {

IR::Type *gen_param_type() {
    std::vector<int64_t> percent = {PCT.PARAMETER_TYPE_BASE,
                                    PCT.PARAMETER_TYPE_STRUCT};
    IR::Type *tp = nullptr;
    std::vector<int64_t> type_probs = {
        PCT.PARAMETER_BASETYPE_BOOL,  PCT.PARAMETER_BASETYPE_ERROR,
        PCT.PARAMETER_BASETYPE_INT,   PCT.PARAMETER_BASETYPE_STRING,
        PCT.PARAMETER_BASETYPE_BIT,   PCT.PARAMETER_BASETYPE_SIGNED_BIT,
        PCT.PARAMETER_BASETYPE_VARBIT};
    switch (randind(percent)) {
    case 0: {
        tp = baseType::pick_rnd_base_type(type_probs);
        break;
    }
    case 1: {
        auto l_types = P4Scope::get_decls<IR::Type_StructLike>();
        if (l_types.size() == 0) {
            return nullptr;
        }
        auto candidate_type = l_types.at(get_rnd_int(0, l_types.size() - 1));
        auto type_name = candidate_type->name.name;
        // check if struct is forbidden
        if (P4Scope::not_initialized_structs.count(type_name) == 0) {
            tp = new IR::Type_Name(candidate_type->name.name);
        } else {
            tp = baseType::pick_rnd_base_type(type_probs);
        }
        break;
    }
    }
    return tp;
}

// Tao: the parameter seems can only be int<> bit<> in bmv2 model
IR::Parameter *parameter::gen(bool if_none_dir) {
    cstring name = randstr(4);
    IR::Type *tp;
    IR::Direction dir;

    tp = gen_param_type();
    if (if_none_dir) {
        dir = IR::Direction::None;
    } else {
        int64_t pct_in = PCT.PARAMETER_DIR_IN;
        int64_t pct_out = PCT.PARAMETER_DIR_OUT;
        int64_t pct_inout = PCT.PARAMETER_DIR_INOUT;
        // It is not possible to have a writable int parameter right now
        if (tp->is<IR::Type_InfInt>()) {
            pct_out = 0;
            pct_inout = 0;
        }
        std::vector<int64_t> percent = {pct_in, pct_out, pct_inout};
        switch (randind(percent)) {
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

    return new IR::Parameter(name, dir, tp);
}

IR::Parameter *parameter::gen_param(IR::Direction dir, cstring p_name,
                                    cstring t_name) {
    IR::Type *tp = new IR::Type_Name(new IR::Path(t_name));
    return new IR::Parameter(p_name, dir, tp);
}

} // namespace CODEGEN

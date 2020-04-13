#include "parameter.h"

#include "baseType.h"
#include "common.h"
#include "scope.h"

namespace CODEGEN {

// Tao: the parameter seems can only be int<> bit<> in bmv2 model
IR::Parameter *parameter::gen(bool if_none_dir) {
    cstring name = randstr(4);
    IR::Type *tp;
    IR::Direction dir;

    if (if_none_dir == false) {
        switch (rand() % 3) {
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
    } else {
        dir = IR::Direction::None;
    }
    std::vector<int> type = {1}; // only bit for now
    tp = baseType::pick_rnd_base_type(type);

    return new IR::Parameter(name, dir, tp);
}

IR::Parameter *parameter::gen_param(IR::Direction dir, cstring p_name,
                                    cstring t_name) {
    IR::Type *tp = new IR::Type_Name(new IR::Path(t_name));
    return new IR::Parameter(p_name, dir, tp);
}

} // namespace CODEGEN

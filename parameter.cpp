#include "parameter.h"

namespace CODEGEN {

// Tao: the parameter seems can only be int<> bit<> in bmv2 model
IR::Parameter *parameter::gen(bool if_none_dir) {
    cstring name = randstr(4);
    IR::Type *tp;
    IR::Direction dir;

    if (if_none_dir == false) {
        switch (rand() % 3) {
        // case 0: dir = IR::Direction::None; break;
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
    std::vector<int> type = {1, 2};

    tp = baseType::gen(false, type);

    return new IR::Parameter(name, dir, tp);
}

IR::Parameter *parameter::gen_ing_headers() {
    IR::Type *tp = new IR::Type_Name(new IR::Path("Headers"));
    return new IR::Parameter("h", IR::Direction::InOut, tp);
}

IR::Parameter *parameter::gen_ing_meta() {
    IR::Type *tp = new IR::Type_Name(new IR::Path("Meta"));
    return new IR::Parameter("m", IR::Direction::InOut, tp);
}

IR::Parameter *parameter::gen_ing_sm() {
    IR::Type *tp = new IR::Type_Name(new IR::Path("standard_metadata_t"));
    return new IR::Parameter("sm", IR::Direction::InOut, tp);
}

IR::Parameter *parameter::gen_pkt_in() {
    IR::Type *tp = new IR::Type_Name(new IR::Path("packet_in"));
    return new IR::Parameter("pkt", IR::Direction::None, tp);
}

IR::Parameter *parameter::gen_out_header() {
    IR::Type *tp = new IR::Type_Name(new IR::Path("Headers"));
    return new IR::Parameter("hdr", IR::Direction::Out, tp);
}

IR::Parameter *parameter::gen_param(IR::Direction dir, cstring p_name,
                                    cstring t_name) {
    IR::Type *tp = new IR::Type_Name(new IR::Path(t_name));
    return new IR::Parameter(p_name, dir, tp);
}

} // namespace CODEGEN

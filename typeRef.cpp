#include "typeRef.h"

namespace CODEGEN {

IR::Type *typeRef::gen() {
    int num_trials = 100;
    IR::Type *tp = nullptr;

    while (num_trials--) {
        int t;
        if (if_random == false) {
            t = type.at(rand() % type.size());
        } else {
            t = rand() % 2;
        }
        switch (t) {
        case 0: {
            if ((for_type == HEADER_UNION) || (for_type == STRUCT_HEADERS)) {
                break;
            }
            std::vector<int> b_types = {1}; // only bit<>
            auto base_type = new baseType(false, b_types);
            tp = base_type->gen();
            break;
        }
        case 1: {
            auto type_name = new typeName(for_type);
            tp = type_name->gen();
            break;
        }
        case 2: {
            if (for_type == HEADER_UNION) {
                break;
            }
            auto hdr_stack = new headerStackType(for_type);
            tp = hdr_stack->gen();
            break;
        }
        }

        if (tp != nullptr) {
            break;
        }
    }

    return tp;
}
} // namespace CODEGEN

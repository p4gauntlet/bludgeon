#include "baseType.h"

namespace CODEGEN {
IR::Type *baseType::gen() {
    int t;

    if (if_random) {
        t = rand() % 4;
    } else {
        t = type.at(rand() % type.size());
    }
    switch (t) {
    case 0: {
        // bool
        return bool_literal::gen();
    }
    case 1: {
        // bit<>
        return bit_literal::gen(false);
    }
    case 2: {
        // int<>, Tao: NOTE here it is bit<>
        return bit_literal::gen(false);
    }
    case 3: {
        return int_literal::gen();
        // int
    }
    }

    return nullptr;
}
} // namespace CODEGEN

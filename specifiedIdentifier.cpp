

#include "specifiedIdentifier.h"

namespace CODEGEN {
IR::IndexedVector<IR::SerEnumMember> specifiedIdentifier::gen(size_t len) {
    IR::IndexedVector<IR::SerEnumMember> members;
    std::set<cstring> members_name;

    for (size_t i = 0; i < len; i++) {
        cstring name = randstr(2);
        auto expr = new expression(); // for bit literal
        IR::Expression *ex = bit_literal::gen_int();

        if (members_name.find(name) != members_name.end()) {
            delete expr;
            delete ex;
            continue;
        }

        auto member = new IR::SerEnumMember(name, ex);

        members.push_back(member);
    }

    return members;
}

} // namespace CODEGEN

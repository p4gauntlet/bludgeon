

#include "headerUnionDeclaration.h"

namespace CODEGEN {
IR::Type_HeaderUnion *headerUnionDeclaration::gen() {
    cstring name = randstr(6);

    IR::IndexedVector<IR::StructField> fields;
    auto l_types = P4Scope::get_decls<IR::Type_Header>();
    if (l_types.size() == 0) {
        return nullptr;
    }
    size_t len = rand() % 5 + 1;
    for (size_t i = 0; i < len; i++) {
        cstring field_name = randstr(4);
        auto hdr_tp = l_types.at(rand() % l_types.size());
        auto tp_name = new IR::Type_Name(hdr_tp->name);
        IR::StructField *sf = new IR::StructField(field_name, tp_name);
        fields.push_back(sf);
    }

    auto ret = new IR::Type_HeaderUnion(name, fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

} // namespace CODEGEN

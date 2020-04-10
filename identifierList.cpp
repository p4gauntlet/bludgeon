

#include "identifierList.h"

namespace CODEGEN {
IR::IndexedVector<IR::Declaration_ID> identifierList::gen(size_t len) {
    IR::IndexedVector<IR::Declaration_ID> decl_ids;
    std::set<cstring> decl_ids_name;

    for (size_t i = 0; i < len; i++) {
        IR::ID *name = new IR::ID(CODEGEN::randstr(2));
        IR::Declaration_ID *decl_id = new IR::Declaration_ID(*name);

        if (decl_ids_name.find(name->name) != decl_ids_name.end()) {
            delete name;
            delete decl_id;
            continue;
        }

        decl_ids.push_back(decl_id);
    }

    return decl_ids;
}

} // namespace CODEGEN

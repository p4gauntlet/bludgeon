#ifndef _TYPEDEFDECLARATION_H_
#define _TYPEDEFDECLARATION_H_

#include "ir/ir.h"

#include "baseType.h"
#include "codegen.h"
#include "common.h"
#include "scope.h"
#include "typeRef.h"

#include "headerTypeDeclaration.h"
#include "headerUnionDeclaration.h"
#include "structTypeDeclaration.h"

namespace CODEGEN {

class typedefDeclaration {
  public:
    const char *types[2] = {"typedef", "type"};

    IR::ID *name = nullptr; // name
    IR::Type *type = nullptr;

    typedefDeclaration() { name = new IR::ID(CODEGEN::randstr(5)); }
    ~typedefDeclaration() {
        if (name != nullptr) {
            delete name;
        }
        if (type != nullptr) {
            delete type;
        }
    }

    void gen_t() {
        while (1) {
            if (rand() % 2 == 0) {
                std::vector<int> types;
                auto type_ref = new typeRef(true, types, "");
                type = type_ref->gen();
            } else {
                std::vector<cstring> names;
                P4Scope::get_all_type_names(STRUCT_LIKE, names);
                auto name = names.at(rand() % names.size());
                type = new IR::Type_Name(new IR::Path(IR::ID(name)));
                auto type = P4Scope::get_type_by_name(name);
                if (type->is<IR::Type_Struct>()) {
                } else if (type->is<IR::Type_Header>()) {
                } else if (type->is<IR::Type_HeaderUnion>()) {
                }
            }

            if (type != nullptr) {
                break;
            }
        }
    }

    void gen_base_t() {
        std::vector<int> b_types;
        auto base_type = new baseType(true, b_types);
        type = base_type->gen();
    }

    IR::Type_Typedef *gen_typedef() {
        gen_t();
        auto ret = new IR::Type_Typedef(*name, type);
        P4Scope::add_to_scope(ret);
        return ret;
    }

    IR::Type_Newtype *gen_newtype() {
        gen_base_t();
        auto ret = new IR::Type_Newtype(*name, type);
        P4Scope::add_to_scope(ret);
        return ret;
    }

    IR::Type *gen() {
        // Tao: we only have typedef now, no newtype
        return gen_typedef();
    }
};

} // namespace CODEGEN

#endif

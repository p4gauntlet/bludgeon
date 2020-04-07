#ifndef _ENUMDECLARATION_H_
#define _ENUMDECLARATION_H_

#include "ir/ir.h"

#include "scope.h"

#include "bit.h"
#include "codegen.h"
#include "identifierList.h"
#include "specifiedIdentifierList.h"

namespace CODEGEN {

class enumDeclaration {
  public:
    const char *types[0] = {};

    int type;
    IR::ID *name = nullptr;
    IR::Type_Bits *tp = nullptr;

    enumDeclaration(int type) : type(type) {
        name = new IR::ID(CODEGEN::randstr(2));
    }
    ~enumDeclaration() {
        delete name;
        if (tp != nullptr) {
            delete tp;
        }
    }

    IR::Type_Enum *gen_enum() {
        auto identifier_l = new identifierList();
        auto decl_ids = identifier_l->gen(3);

        auto ret = new IR::Type_Enum(*name, decl_ids);

        P4Scope::add_to_scope(ret);
        return ret;
    }

    IR::Type_SerEnum *gen_ser_enum() {
        auto spec_l = new specifiedIdentifierList();
        auto members = spec_l->gen(3);
        tp = bit_literal::gen(false);

        auto ret = new IR::Type_SerEnum(*name, tp, members);

        P4Scope::add_to_scope(ret);
        return ret;
    }

    IR::Type *gen() {
        if (type == 0) {
            return gen_enum();
        } else {
            return gen_ser_enum();
        }
    }
};

} // namespace CODEGEN

#endif

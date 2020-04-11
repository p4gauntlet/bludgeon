

#include "typeDeclaration.h"

#include "common.h"
#include "structTypeDeclaration.h"
#include "headerUnionDeclaration.h"
#include "headerTypeDeclaration.h"
#include "headerStackType.h"
#include "enumDeclaration.h"


namespace CODEGEN {

IR::Type_Declaration *typeDeclaration::gen() {
    std::vector<int> percent = {50, 50, 0};
    IR::Type_Declaration *decl = nullptr;
    bool use_default_decl = false;
    switch (randind(percent)) {
    case 0: {
        use_default_decl = true;
        break;
    }
    case 1: {
        decl = structTypeDeclaration::gen();
        break;
    }
    case 2: {
        // header unions are disabled for now, need to fix assignments
        auto hdrs = P4Scope::get_decls<IR::Type_Header>();
        // we can only generate a union if we have at least two headers
        if (hdrs.size() > 1) {
            decl = headerUnionDeclaration::gen();
            if (not decl) {
                use_default_decl = true;
            }
        } else {
            use_default_decl = true;
        }
        break;
    }
    }
    if (use_default_decl)
        decl = headerTypeDeclaration::gen();

    return decl;
}


} // namespace CODEGEN

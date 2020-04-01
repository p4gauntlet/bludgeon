#include "keyElement.h"
#include "expression_2.h"
namespace CODEGEN {
IR::KeyElement *keyElement::gen(cstring match_kind) {
    auto annotat       = new Annotations();
    auto match         = new IR::PathExpression(match_kind);
    auto annotations   = annotat->gen();
    // TODO: how to generate meaningful expr
    auto bit_type = P4Scope::pick_declared_bit_type(false);

    // Ideally this should have a fallback option
    if (not bit_type) {
        printf("Could not find key lval for key matches\n");
        return nullptr;
    }
    // for some reason expression do not work here?
    // TODO: Fix
    auto expr = expression2::gen_expr(bit_type, true);
    auto key =new IR::KeyElement(annotations, expr, match);

    return key;
}
} // namespace CODEGEN

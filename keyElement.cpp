#include "keyElement.h"
#include "expression.h"
namespace CODEGEN {
IR::KeyElement *keyElement::gen(cstring match_kind) {
    auto match = new IR::PathExpression(match_kind);
    auto annotations = Annotations::gen();
    // TODO: how to generate meaningful expr
    auto bit_type = P4Scope::pick_declared_bit_type(false);

    // Ideally this should have a fallback option
    if (not bit_type) {
        printf("Could not find key lval for key matches\n");
        return nullptr;
    }
    // this expression cannot be an infinite precision integer
    P4Scope::req.require_scalar = true;
    P4Scope::req.compile_time_known = true;
    auto expr = expression::gen_expr(bit_type);
    auto key = new IR::KeyElement(annotations, expr, match);

    return key;
}
} // namespace CODEGEN

#include "keyElementList.h"

namespace CODEGEN {
IR::Key *keyElementList::gen(size_t len) {
    IR::Vector<IR::KeyElement> keys;

    for (size_t i = 0; i < len; i++) {
        // TODO: More types than just exact
        IR::KeyElement *key = keyElement::gen("exact");
        if (not key) {
            continue;
        }
        // @name
        // Tao: actually, this may never happen
        auto key_anno   = key->annotations->annotations.at(0);
        auto annot_expr = key_anno->expr.at(0);
        cstring key_annotat_name;
        if (annot_expr->is<IR::StringLiteral>()) {
            auto str_expr = annot_expr->to<IR::StringLiteral>();
            key_annotat_name = str_expr->value;
        } else {
            BUG("must be a string literal");
        }

        keys.push_back(key);
    }


    return new IR::Key(keys);
}
} // namespace CODEGEN

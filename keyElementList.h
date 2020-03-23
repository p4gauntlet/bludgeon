#ifndef _KEYELEMENTLIST_H_
#define _KEYELEMENTLIST_H_

#include "ir/ir.h"


#include "keyElement.h"


namespace CODEGEN {
class keyElementList {
public:
    const char *types[0] = {
    };

    IR::Vector<IR::Expression> key_exprs;
    std::set<cstring> key_annotat_names;

    keyElementList() {
    }

    ~keyElementList() {
    }

    IR::Key *gen(size_t len) {
        IR::Vector<IR::KeyElement> keys;

        for (size_t i = 0; i < len; i++) {
            auto key_ele        = new keyElement();
            IR::KeyElement *key = key_ele->gen();

            if (key == nullptr) {
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
            } else   {
                BUG("must be a string literal");
            }

            if (key_annotat_names.find(key_annotat_name)
                != key_annotat_names.end()) {
                continue;
            }

            auto key_expr = key->expression;

            bool found_same_key = false;
            for (size_t i = 0; i < key_exprs.size(); i++) {
                if (key_exprs.at(i) == key_expr) {
                    found_same_key = true;
                    break;
                }
            }

            if (found_same_key == true) {
                continue;
            }

            key_exprs.push_back(key_expr);
            keys.push_back(key);
        }


        return new IR::Key(keys);
    }
};
} // namespace CODEGEN



#endif

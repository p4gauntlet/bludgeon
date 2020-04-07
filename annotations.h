#ifndef _ANNOTATIONS_H_
#define _ANNOTATIONS_H_

#include "codegen.h"
#include "expression.h"
#include "ir/ir.h"

namespace CODEGEN {
class Annotations {
  public:
    const char *types[0] = {};

    IR::Vector<IR::Annotation> annotations;

    IR::ID *name;
    IR::Vector<IR::Expression> exprs;
    // Tao: not neccessary right now
    // IR::Vector< IR::AnnotationToken > annotat_tokens;

    Util::SourceInfo si;

    Annotations() { name = new IR::ID(IR::Annotation::nameAnnotation); }

    IR::Annotations *gen() {
        auto str_literal = new IR::StringLiteral(CODEGEN::randstr(6));

        exprs.push_back(str_literal);

        auto annotation = new IR::Annotation(si, *name, exprs, false);
        annotations.push_back(annotation);

        return new IR::Annotations(annotations);
    }
};
} // namespace CODEGEN

#endif

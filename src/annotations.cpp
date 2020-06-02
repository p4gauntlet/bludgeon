#include "annotations.h"

#include "common.h"

namespace CODEGEN {
IR::Annotations *Annotations::gen() {
    Util::SourceInfo si;
    IR::Vector<IR::Annotation> annotations;
    IR::Vector<IR::Expression> exprs;
    cstring name = IR::Annotation::nameAnnotation;
    auto str_literal = new IR::StringLiteral(randstr(6));

    exprs.push_back(str_literal);

    auto annotation = new IR::Annotation(si, name, exprs, false);
    annotations.push_back(annotation);

    return new IR::Annotations(annotations);
}
} // namespace CODEGEN

#ifndef _ANNOTATIONS_H_
#define _ANNOTATIONS_H_

#include "ir/ir.h"


#include "codegen.h"

#include "expression.h"

namespace CODEGEN {

class Annotations {
public:
	const char *types[0] = {
	};

	IR::Vector< IR::Annotation > annotations;

	IR::ID* name;
	IR::Vector< IR::Expression > exprs;
	// Tao: not neccessary right now
	// IR::Vector< IR::AnnotationToken > annotat_tokens;

	Util::SourceInfo si;

	Annotations() {
		name = new IR::ID(IR::Annotation::nameAnnotation);
	}

	IR::Annotations* gen() {
		auto expr_gen = new expression(2); // StringLiteral expr
		auto str_literal = expr_gen->gen();
		exprs.push_back(str_literal);

		auto annotation = new IR::Annotation(si, *name, exprs);
		annotations.push_back(annotation);

		return new IR::Annotations(annotations);
	}
};


} // namespace CODEGEN


#endif

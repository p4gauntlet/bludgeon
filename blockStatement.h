#ifndef _BLOCKSTATEMENT_H_
#define _BLOCKSTATEMENT_H_

#include "ir/ir.h"

#include "constantDeclaration.h"


namespace CODEGEN {


class blockStatement {
public:
	const char* types[0] = {
	};

	// Tao: please take care of the scope thing
	IR::IndexedVector< IR::StatOrDecl > stat_or_decls;

	blockStatement() {
	}

	// TODO: a lot of work needed to be done here
	IR::BlockStatement* gen() {
		auto const_decl = new constantDeclaration();
		stat_or_decls.push_back(const_decl->gen());


		return new IR::BlockStatement(stat_or_decls);
	}

};


} // namespace CODEGEN




#endif

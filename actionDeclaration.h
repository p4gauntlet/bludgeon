#ifndef _ACTIONDECLARATION_H_
#define _ACTIONDECLARATION_H_

#include "ir/ir.h"
#include "parameterList.h"
#include "blockStatement.h"

namespace CODEGEN {


class actionDeclaration {
public:
	const char* types[0] = {
	};


	IR::ID* name = nullptr;
	IR::ParameterList* params = nullptr;
	IR::BlockStatement* blk = nullptr;

	actionDeclaration() {
		name = new IR::ID(CODEGEN::randstr(5));
	}

	IR::P4Action* gen() {
        P4Scope::start_local_scope();
		auto param_gen = new parameterList(false);
		params = param_gen->gen();

		auto blk_gen = new blockStatement();
		blk = blk_gen->gen();

		auto ret = new IR::P4Action(*name, params, blk);


        P4Scope::end_local_scope();


		P4Scope::add_to_scope(ret);
		return ret;
	}
};


} // namespace CODEGEN




#endif

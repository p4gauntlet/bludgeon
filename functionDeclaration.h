#ifndef _FUNCTIONDECLARATION_H_
#define _FUNCTIONDECLARATION_H_


#include "scope.h"
#include "expression.h"
#include "blockStatement.h"
#include "parameterList.h"

#include "ir/ir.h"

namespace CODEGEN {


class functionDeclaration {
public:
	const char* types[0] = {
	};

	IR::ID* name;
	IR::Type_Method* tm = nullptr;
	IR::BlockStatement* blk = nullptr;

	functionDeclaration() {
		name = new IR::ID(CODEGEN::randstr(7));
	}
	~functionDeclaration() {
		delete name;
		if (tm != nullptr) {
			delete tm;
		}
		if (blk != nullptr) {
			delete blk;
		}
	}

	IR::Function* gen() {
		P4Scope::start_local_scope();
		auto param_list_gen = new parameterList(false);
		IR::ParameterList* params = param_list_gen->gen();

		IR::Type_Bits * r_tp = new IR::Type_Bits(2<<(rand()%4), false);
		tm = new IR::Type_Method(r_tp, params);

		P4Scope::ret_type = r_tp;
		std::vector<cstring> tab_names;
		auto blk_gen = new blockStatement(tab_names, true);
		blk = blk_gen->gen_func_blk(false);
		P4Scope::ret_type = nullptr;

		auto ret = new IR::Function(*name, tm, blk);
		P4Scope::end_local_scope();
		P4Scope::add_to_scope(ret);
		return ret;
	}

};


} // namespace CODEGEN




#endif

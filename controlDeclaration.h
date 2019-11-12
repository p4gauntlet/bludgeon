#ifndef _CONTROLDECLARATION_H_
#define _CONTROLDECLARATION_H_

#include "ir/ir.h"


#include "scope.h"
#include "controlTypeDeclaration.h"
#include "constantDeclaration.h"
#include "blockStatement.h"

namespace CODEGEN {


class controlDeclaration {
public:
	const char* types[0] = {
	};

	IR::ID* name;
	IR::Type_Control* type_ctrl;
	IR::ParameterList* constructor_params;
	IR::IndexedVector< IR::Declaration > local_decls;
	IR::BlockStatement* blk;


	controlDeclaration() {
		name = new IR::ID(CODEGEN::randstr(7));
	}

	IR::P4Control* gen() {
		// type_ctrl
		auto ctrl_type_decl = new controlTypeDeclaration();
		type_ctrl = ctrl_type_decl->gen();

		// constructor_params
		auto param_gen = new parameterList(true);
		constructor_params = param_gen->gen();
		// local_decls
		auto const_decl = new constantDeclaration();
		local_decls.push_back(const_decl->gen());


		// blockstatement
		auto blk_gen = new blockStatement();
		blk = blk_gen->gen();


		return new IR::P4Control((type_ctrl->name), type_ctrl, constructor_params, local_decls, blk);

	}
};


} // namespace CODEGEN




#endif

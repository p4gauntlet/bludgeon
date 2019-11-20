#ifndef _CONTROLDECLARATION_H_
#define _CONTROLDECLARATION_H_

#include "ir/ir.h"


#include "scope.h"
#include "controlTypeDeclaration.h"
#include "constantDeclaration.h"
#include "variableDeclaration.h"
#include "actionDeclaration.h"
#include "tableDeclaration.h"
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
	~controlDeclaration() {
		delete name;
		delete type_ctrl;
		// delete constructor_params;
		delete blk;
	}

	IR::P4Control* gen() {
        // start of new scope
        P4Scope::start_local_scope();
		// type_ctrl
		auto ctrl_type_decl = new controlTypeDeclaration();
		type_ctrl = ctrl_type_decl->gen();

		// constructor_params
		// auto param_gen = new parameterList(true);
		// constructor_params = param_gen->gen();
        
		// local_decls
        // constantDeclarations
        for (int i=0; i<5; i++) {
		    auto const_decl = new constantDeclaration();
		    local_decls.push_back(const_decl->gen());
        }
        // variableDeclarations
        for (int i=0; i<5; i++) {
            auto var_decl = new variableDeclaration();
            local_decls.push_back(var_decl->gen());
        }
        // actionDeclarations
        for (int i=0; i<5; i++) {
            auto act_decl = new actionDeclaration();
            local_decls.push_back(act_decl->gen());
        }
        // tableDeclarations
        for (int i=0; i<5; i++) {
            auto tab_decl = new tableDeclaration();
            local_decls.push_back(tab_decl->gen());
        }
        // instantiations


		// blockstatement
		auto blk_gen = new blockStatement();
		blk = blk_gen->gen();

		std::cout << expression::construct_op_expr() << std::endl;
		std::cout << expression::construct_op_expr() << std::endl;
		std::cout << expression::construct_op_expr() << std::endl;
		std::cout << expression::construct_op_expr() << std::endl;
		std::cout << expression::construct_op_expr() << std::endl;

		// return new IR::P4Control((type_ctrl->name), type_ctrl, constructor_params, local_decls, blk);
        // end of scope
        P4Scope::end_local_scope();
		return new IR::P4Control((type_ctrl->name), type_ctrl, local_decls, blk);

	}

	static IR::P4Control* gen_ing_ctrl() {
        // start of new scope
        P4Scope::start_local_scope();


		//
		IR::ID* name = new IR::ID("ingress");
		IR::Type_Control* type_ctrl = controlTypeDeclaration::gen_ing_ctrl_type();
		// IR::ParameterList* constructor_params;
		IR::IndexedVector< IR::Declaration > local_decls;
		IR::BlockStatement *blk = new IR::BlockStatement();


        // end of scope
        P4Scope::end_local_scope();

		return new IR::P4Control(*name, type_ctrl, local_decls, blk);
	}
};


} // namespace CODEGEN




#endif

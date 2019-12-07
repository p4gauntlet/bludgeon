#ifndef _CONTROLDECLARATION_H_
#define _CONTROLDECLARATION_H_

#include "ir/ir.h"


#include "scope.h"
#include "expression.h"
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
		/* Tao: No constant declarations
        for (int i=0; i<5; i++) {
		    auto const_decl = new constantDeclaration();
		    local_decls.push_back(const_decl->gen());
        }*/
        // variableDeclarations
        for (int i=0; i<15; i++) {
            auto var_decl = new variableDeclaration();
            local_decls.push_back(var_decl->gen());
        }
		// declaration_instance
		for (int i=0; i<5; i++) {
			auto decl_ins = gen_decl_instance();
			if (decl_ins == nullptr) {
				continue;
			}
			local_decls.push_back(decl_ins);
		}
        // actionDeclarations
        for (int i=0; i<4; i++) {
            auto act_decl = new actionDeclaration();
            local_decls.push_back(act_decl->gen());
        }
        // tableDeclarations
		std::vector<cstring> tab_names;
        for (int i=0; i<4; i++) {
            auto tab_decl = new tableDeclaration();
            local_decls.push_back(tab_decl->gen());
			tab_names.push_back(tab_decl->name->name);
        }
        // instantiations


		// blockstatement
		auto blk_gen = new blockStatement(tab_names, true);
		blk = blk_gen->gen();

		// NOTE::::: this is for test, not actual code fragments
		/*
		int xxx = 5;
		while (xxx--) {
			IR::Expression* pppp = expression::construct_compound_cond_expr();
			if (pppp != nullptr) {
				std::cout << pppp << std::endl;
			}
			pppp = expression::construct_cond_expr();
			if (pppp != nullptr) {
				std::cout << pppp << std::endl;
			}
			pppp = expression::construct_op_expr();
			if (pppp != nullptr) {
				std::cout << pppp << std::endl;
			}
		}*/

        // end of scope
        P4Scope::end_local_scope();
		// add to the whole scope
		IR::P4Control *p4ctrl = new IR::P4Control((type_ctrl->name), type_ctrl, local_decls, blk);
		P4Scope::add_to_scope(p4ctrl);
		P4Scope::p4_ctrls.push_back(p4ctrl);
		return p4ctrl;

	}

	static IR::P4Control* gen_ing_ctrl() {
        // start of new scope
        P4Scope::start_local_scope();
		//
		IR::ID* name = new IR::ID("ingress");
		IR::Type_Control* type_ctrl = controlTypeDeclaration::gen_ing_ctrl_type();
		// IR::ParameterList* constructor_params;
		IR::IndexedVector< IR::Declaration > local_decls;
		// local_decls
        // constantDeclarations
		/* Tao: no constant here
        for (int i=0; i<5; i++) {
		    auto const_decl = new constantDeclaration();
		    local_decls.push_back(const_decl->gen());
        }*/
        // variableDeclarations
        for (int i=0; i<15; i++) {
            auto var_decl = new variableDeclaration();
            local_decls.push_back(var_decl->gen());
        }
		// declaration_instance
		for (int i=0; i<5; i++) {
			auto decl_ins = gen_decl_instance();
			if (decl_ins == nullptr) {
				continue;
			}
			local_decls.push_back(decl_ins);
		}
        // actionDeclarations
        for (int i=0; i<5; i++) {
            auto act_decl = new actionDeclaration();
            local_decls.push_back(act_decl->gen());
        }
        // tableDeclarations
		std::vector<cstring> tab_names;
        for (int i=0; i<5; i++) {
            auto tab_decl = new tableDeclaration();
            local_decls.push_back(tab_decl->gen());
			tab_names.push_back(tab_decl->name->name);
        }
        // instantiations


		// blockstatement
		auto blk_gen = new blockStatement(tab_names, true);
		auto blk_stat = blk_gen->gen();


        // end of scope
        P4Scope::end_local_scope();
		// add to the whole scope
		IR::P4Control *p4ctrl = new IR::P4Control(*name, type_ctrl, local_decls, blk_stat);
		P4Scope::add_to_scope(p4ctrl);
		P4Scope::p4_ctrls.push_back(p4ctrl);
		return p4ctrl;
	}

	static IR::Declaration_Instance* gen_decl_instance() {
		size_t size = P4Scope::p4_ctrls.size();
		if (size == 0) {
			return nullptr;
		}
		IR::Declaration_Instance* decl_ins = nullptr;
		IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
		IR::P4Control* p4ctrl = P4Scope::p4_ctrls.at(rand()%size);
		IR::Type* tp = new IR::Type_Name(new IR::Path(p4ctrl->name));

		IR::ID *name = new IR::ID(CODEGEN::randstr(6));
		decl_ins = new IR::Declaration_Instance(*name, tp, args);

		P4Scope::decl_ins_ctrls.emplace(name->name, p4ctrl);

		return decl_ins;
	}
};


} // namespace CODEGEN




#endif

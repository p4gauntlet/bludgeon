#include "blockStatement.h"



namespace CODEGEN {

void blockStatement::initialization() {
	// initialize out parameters
	auto ass_stats = expression::decl_p_initialize();
	for (auto i : ass_stats) {
		stat_or_decls.push_back(i);
	}
	// initialize variables
	if (if_v_initialize == true) {
		auto ass_stats = expression::decl_v_initialize();
		for (auto i : ass_stats) {
			stat_or_decls.push_back(i);
		}
	}
}

void blockStatement::gen_sth() {

	// put tab_name .apply() after some initializations
    for (int num_stat=0; num_stat<NUM_STAT; num_stat++) {
        switch (rand()%10) {
        case 0: {
            std::vector<cstring> tab_names = P4Scope::get_tab_names();
            if (tab_names.size() > 0) {
                IR::Vector<IR::Argument> * args = new IR::Vector<IR::Argument>();
                auto tab_name = tab_names.at(rand()%tab_names.size());
                if (P4Scope::called_tables.find(tab_name) ==
                        P4Scope::called_tables.end()) {
                    auto mem = new IR::Member(new IR::PathExpression(new IR::Path(IR::ID(tab_name))), IR::ID("apply"));
                    auto mce = new IR::MethodCallExpression(mem, args);
                    stat_or_decls.push_back(new IR::MethodCallStatement(mce));
                    P4Scope::called_tables.insert(tab_name);
                }
            }
            break;
        }
        case 1: {
            switchStatement *switch_stat_gen = new switchStatement();
            auto switch_stat = switch_stat_gen->gen();
            if (switch_stat != nullptr) {
                stat_or_decls.push_back(switch_stat);
            }
            break;
        }
        case 2: {
	    	auto ass = assignmentOrMethodCallStatement::gen_assignstat();
	    	if (ass != nullptr)
	    		stat_or_decls.push_back(ass);
            break;
        }
        case 3: {
	    	auto func_ass = assignmentOrMethodCallStatement::gen_func_ass();
	    	if (func_ass != nullptr)
	    		stat_or_decls.push_back(func_ass);
            break;
        }
        case 4: {
	    	auto compound_ass = assignmentOrMethodCallStatement::gen_compound_ass();
	    	if (compound_ass != nullptr)
	    		stat_or_decls.push_back(compound_ass);
            break;
        }
        case 5: {
	        /*
	         * Tao: note here, may be a lot recursions,
	         *		so add some randomness here
	         */
	        if (rand()%4 == 0) {
	        	auto if_stat = conditionalStatement::gen_if_stat();
	        	if (if_stat != nullptr) 
	        		stat_or_decls.push_back(if_stat);
	        }
            break;
        }
        case 6: {
            // Tao: generate action method call statement
	    	auto mcs = assignmentOrMethodCallStatement::gen_act_methodcall_stat();
	    	if (mcs != nullptr) {
	    		stat_or_decls.push_back(mcs);
	    	}
            break;
        }
        case 7: {
	        // Tao: gen ctrl method call statement
	        if (if_in_ifs == false) {
	        	auto mcs = assignmentOrMethodCallStatement::gen_methodcall_stat();
	        	if (mcs != nullptr) {
	        		stat_or_decls.push_back(mcs);
	        	}
	        }
            break;
        }
        case 8: {
			stat_or_decls.push_back(returnStatement::gen_ret_stat());
            break;
        }
        case 9: { 
			stat_or_decls.push_back(exitStatement::gen()); 
            break;
        }
        }
    }

    // Tao: make sure all declared tables are used
    std::vector<cstring> tab_names = P4Scope::get_tab_names();
    for (size_t i=0; i<tab_names.size(); i++) {
        if (P4Scope::called_tables.find(tab_names.at(i)) == 
                P4Scope::called_tables.end()) {
            IR::Vector<IR::Argument> * args = new IR::Vector<IR::Argument>();
            auto tab_name = tab_names.at(i);
            auto mem = new IR::Member(new IR::PathExpression(new IR::Path(IR::ID(tab_name))), IR::ID("apply"));
            auto mce = new IR::MethodCallExpression(mem, args);
            stat_or_decls.push_back(new IR::MethodCallStatement(mce));
        }
    }
}

IR::BlockStatement* blockStatement::gen() {

    initialization();
	gen_sth();

	return new IR::BlockStatement(stat_or_decls);
}

IR::BlockStatement* blockStatement::gen_switch_blk() {
    gen_sth();
    return new IR::BlockStatement(stat_or_decls);
}

IR::BlockStatement* blockStatement::gen_func_blk() {
	// some variable declaration
	for (int i=0; i<5; i++) {
        auto var_decl = new variableDeclaration();
        stat_or_decls.push_back(var_decl->gen());
    }

    initialization();
	gen_sth();


	if (P4Scope::ret_type != nullptr) {
		const IR::Type_Bits *tp_bits = P4Scope::ret_type->to<IR::Type_Bits>();

		IR::Expression* expr = nullptr;
		int num_trials = 100;
		while (num_trials--) {
			expr = expression::construct_op_expr();
			if (expr != nullptr) {
				break;
			}
		}
		IR::ReturnStatement *ret_stat;
		if (expr == nullptr) {
			ret_stat = new IR::ReturnStatement(new IR::Constant(0));
		}
		else {
			ret_stat = new IR::ReturnStatement(new IR::Cast(tp_bits, expr));
		}
		stat_or_decls.push_back(ret_stat);
	}

	return new IR::BlockStatement(stat_or_decls);
}

} // namespace CODEGEN


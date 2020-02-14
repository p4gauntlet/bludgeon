#ifndef _P4STATE_H_
#define _P4STATE_H_


#include "ir/ir.h"

#include "common.h"
#include "scope.h"
#include "expression.h"
#include "assignmentOrMethodCallStatement.h"
#include "variableDeclaration.h"



namespace CODEGEN {

class p4State {
public:

    std::map<const cstring, const IR::Type *> hdr_fields_types;
    std::vector<cstring> hdr_fields_names;
    std::vector<cstring> states;

    p4State() {
        if (P4Scope::sys_hdr->name.name != "Headers") {
            BUG("sys hdr name is not correct %1%!", P4Scope::sys_hdr->name.name);
        }

        for (auto sf : P4Scope::sys_hdr->fields) {
            hdr_fields_names.push_back(sf->name.name);
            hdr_fields_types.emplace(sf->name.name, sf->type);
        }

        states = {
            "state_0",
            "state_1",
            "state_2",
            "state_3",
            "state_4",
        };
    }

    ~p4State() {
    }

    IR::IndexedVector<IR::ParserState> gen_states() {
        IR::IndexedVector<IR::ParserState> ret;
        ret.push_back(gen_start_state());
        for (auto name : states) {
            ret.push_back(gen_state(name));
        }
        return ret;
    }

    IR::MethodCallStatement* gen_methodcall_stat(cstring field_name) {
        auto pkt_call = new IR::Member(new IR::PathExpression(new IR::Path(IR::ID("pkt"))),
                                            IR::ID("extract"));

        IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();

        auto field_type = hdr_fields_types[field_name];

        auto mem = new IR::Member(new IR::PathExpression(new IR::Path(IR::ID("hdr"))), 
                                            IR::ID(field_name));

        if (field_type->is<IR::Type_Stack>()) {
            IR::Argument *arg;
            auto tp_stack = field_type->to<IR::Type_Stack>();
            auto size = tp_stack->size->to<IR::Constant>()->value;
            auto arr_index = new IR::ArrayIndex(mem, new IR::Constant(rand()%size));
    
            if (tp_stack->elementType->is<IR::Type_Name>()) {
                auto tp_name = tp_stack->elementType->to<IR::Type_Name>();
                auto real_tp = P4Scope::get_type_by_name(tp_name->path->name.name);
                if (real_tp->is<IR::Type_HeaderUnion>()) {
                    auto tp_hdr_union = real_tp->to<IR::Type_HeaderUnion>();
                    auto rand_ind = rand()%(tp_hdr_union->fields.size());
                    auto sf = tp_hdr_union->fields.at(rand_ind);
                    arg = new IR::Argument(new IR::Member(arr_index, IR::ID(sf->name.name)));
                }
                else {
                    arg = new IR::Argument(arr_index);
                }
            }
            else {
                arg = new IR::Argument(arr_index);
            }

            args->push_back(arg);
        }
        else {
            IR::Argument *arg;
            auto field_ind = std::find(hdr_fields_names.begin(),
                                        hdr_fields_names.end(),
                                        field_name);
            hdr_fields_names.erase(field_ind);

            if (field_type->is<IR::Type_Name>()) {
                auto tp_name = field_type->to<IR::Type_Name>();
                auto real_tp = P4Scope::get_type_by_name(tp_name->path->name.name);
                if (real_tp->is<IR::Type_HeaderUnion>()) {
                    auto tp_hdr_union = real_tp->to<IR::Type_HeaderUnion>();
                    auto rand_ind = rand()%(tp_hdr_union->fields.size());
                    auto sf = tp_hdr_union->fields.at(rand_ind);
                    arg = new IR::Argument(new IR::Member(mem, IR::ID(sf->name.name)));
                }
                else {
                    arg = new IR::Argument(mem);
                }
            }
            else {
                arg = new IR::Argument(mem);
            }
            args->push_back(arg);
        }

        auto mce = new IR::MethodCallExpression(pkt_call, args);
        return new IR::MethodCallStatement(mce);
    }

    IR::ParserState * gen_start_state() {
        IR::Expression* transition;
        IR::IndexedVector<IR::StatOrDecl> components;
        IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
        auto pkt_call = new IR::Member(new IR::PathExpression(new IR::Path(IR::ID("pkt"))),
                                            IR::ID("extract"));

        IR::Argument *arg = new IR::Argument(new IR::Member(new IR::PathExpression(new IR::Path(IR::ID("hdr"))), IR::ID(ETH_HDR)));
        args->push_back(arg);
        auto mce = new IR::MethodCallExpression(pkt_call, args);
        components.push_back(new IR::MethodCallStatement(mce));

        transition = new IR::PathExpression(new IR::Path(IR::ID("state_0")));

        auto ret = new IR::ParserState(IR::ID("start"), components, transition);
        P4Scope::add_to_scope(ret);
        return ret;
    }

    IR::ParserState * gen_state(cstring state_name) {
        IR::ID name(state_name);
        IR::IndexedVector<IR::StatOrDecl> components;

        P4Scope::start_local_scope();

        // variable decls
        for (int i=0; i<5; i++) {
            auto var_decl = new variableDeclaration();
            components.push_back(var_decl->gen());
        }
        // statements
        for (int i=0; i<5; i++) {
            switch(rand()%3) {
            case 0: {
	    	    auto ass = assignmentOrMethodCallStatement::gen_assignstat();
	    	    if (ass != nullptr)
	    	    	components.push_back(ass);
                break;
            }
            /* Tao: may be some if statements in function
            case 1: {
	    	    auto func_ass = assignmentOrMethodCallStatement::gen_func_ass();
	    	    if (func_ass != nullptr)
	    	    	components.push_back(func_ass);
                break;
            }*/
            case 1: {
	    	    auto compound_ass = assignmentOrMethodCallStatement::gen_compound_ass();
	    	    if (compound_ass != nullptr)
	    	    	components.push_back(compound_ass);
                break;
            }
            case 2: {
                // extract
                if (hdr_fields_names.size() != 0) {
                    auto field_name = hdr_fields_names.at(rand()%hdr_fields_names.size());
                    components.push_back(gen_methodcall_stat(field_name));
                }
                break;
            } 
            }
        }

        // expression
        IR::Expression *transition;
        IR::Vector<IR::Expression> exprs;
        IR::Vector<IR::SelectCase> cases;
        std::vector<const IR::Type*> types;

        switch(rand()%4) {
        case 0: {
            transition = new IR::PathExpression(new IR::Path(IR::ID("accept")));
            break;
        }
        case 1: {
            transition = new IR::PathExpression(new IR::Path(IR::ID("reject")));
            break;
        }
        case 2: {
            transition = new IR::PathExpression(new IR::Path(IR::ID(states.at(rand()%states.size()))));
            break;
        }
        case 3: {
            size_t num = rand()%3+1;
            auto get_le_flag = expression::get_list_expressions(exprs, types, num);
            if (get_le_flag == true) {

                for (int i=0; i<rand()%3+1; i++) {
                    IR::Expression *keyset;
                    IR::Vector<IR::Expression> args; 
                    for (size_t i=0; i<types.size(); i++) {
                        auto tp = types.at(i)->to<IR::Type_Bits>();
                        int size = tp->size>4?4:tp->size;
                        int val = rand()%(1<<size);
                        args.push_back(new IR::Constant(val));
                    }
                    keyset = new IR::ListExpression(args);

                    auto sw_case = new IR::SelectCase(keyset, new IR::PathExpression(new IR::Path(IR::ID(states.at(rand()%states.size())))));
                    cases.push_back(sw_case);
                }
                if (rand()%2 == 0) {
                    cases.push_back(new IR::SelectCase(new IR::DefaultExpression, new IR::PathExpression(new IR::Path(IR::ID("accept")))));
                }
                else {
                    cases.push_back(new IR::SelectCase(new IR::DefaultExpression, new IR::PathExpression(new IR::Path(IR::ID("reject")))));
                }

                transition = new IR::SelectExpression(new IR::ListExpression(exprs), cases);
            }
            else {
                transition = new IR::PathExpression(new IR::Path(IR::ID(states.at(rand()%states.size()))));
            }
            break;
        }
        }


        P4Scope::end_local_scope();
        // add to scope
        auto ret = new IR::ParserState(name, components, transition);
        P4Scope::add_to_scope(ret);
        return ret;
    }


}; // class p4State


} // namespace CODEGEN



#endif

#ifndef _P4PARSER_H_
#define _P4PARSER_H_

#include "ir/ir.h"

#include "common.h"
#include "scope.h"
#include "parameterList.h"
#include "p4state.h"


namespace CODEGEN {


class p4Parser {

public:

    IR::ID* p_name;
    IR::Type_Parser* tp_parser;
    IR::IndexedVector<IR::Declaration> parserLocals;
    IR::IndexedVector<IR::ParserState> states;

    p4Parser() {
    }

    ~p4Parser() {
        delete p_name;
        delete tp_parser;
    }

    IR::P4Parser *gen_sys_p() {
        P4Scope::start_local_scope();

        // generate type_parser
        p_name = new IR::ID("p");
        auto param_list = parameterList::gen_par_params();
        auto params = param_list->parameters;
        // add to the scope
        for (size_t i=0; i<params.size(); i++) {
            auto param = params.at(i);
            P4Scope::add_to_scope((IR::Node *)param);
            if (param->direction != IR::Direction::In) {
                P4Scope::add_name_2_type_p(param->name.toString(), param->type);
            }
        }

        tp_parser = new IR::Type_Parser(*p_name, param_list);
        // generate decls
        for (int i=0; i<5; i++) {
            auto var_decl = new variableDeclaration();
            parserLocals.push_back(var_decl->gen());
        }

        // generate states
        auto p4stats = new p4State();
        states = p4stats->gen_states();

        P4Scope::end_local_scope();
        // add to the whole scope
        auto p4parser = new IR::P4Parser(*p_name, tp_parser, parserLocals, states);
        P4Scope::add_to_scope(p4parser);
        return p4parser;
    }
};


} // namesapce CODEGEN







#endif

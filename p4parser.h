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

    IR::ID *p_name;
    IR::Type_Parser *tp_parser;
    IR::IndexedVector<IR::Declaration>parserLocals;
    IR::IndexedVector<IR::ParserState>states;

    p4Parser() {}

    ~p4Parser() {
        delete p_name;
        delete tp_parser;
    }

    IR::P4Parser* gen_sys_p() {
        P4Scope::start_local_scope();

        // generate type_parser
        p_name = new IR::ID("p");
        auto param_list = parameterList::gen_par_params();
        auto params     = param_list->parameters;

        // add to the scope
        for (size_t i = 0; i < params.size(); i++) {
            auto param = params.at(i);
            P4Scope::add_to_scope((IR::Node *)param);

            if (param->direction != IR::Direction::In) {
                P4Scope::add_name_2_type_p(param->name.toString(), param->type);
            }
        }

        tp_parser = new IR::Type_Parser(*p_name, param_list);

        // generate decls
        for (int i = 0; i < 5; i++) {
            auto var_decl = variableDeclaration::gen();
            parserLocals.push_back(var_decl);
        }

        // generate states
        auto p4state = new p4State();
        IR::IndexedVector<IR::ParserState> states;
        states.push_back(p4state->gen_start_state());
        states.push_back(p4state->gen_hdr_states());

        P4Scope::end_local_scope();

        // add to the whole scope
        auto p4parser = new IR::P4Parser(*p_name,
                                         tp_parser,
                                         parserLocals,
                                         states);
        P4Scope::add_to_scope(p4parser);
        return p4parser;
    }

    IR::P4Parser* gen_tofino_p() {
        P4Scope::start_local_scope();

        // generate type_parser
        p_name = new IR::ID("SwitchIngressParser");
        IR::IndexedVector<IR::Parameter> params;

        IR::Parameter *param = parameter::gen_param(IR::Direction::None, "pkt", "packet_in");
        params.push_back(param);
        param = parameter::gen_param(IR::Direction::Out, "hdr", "Headers");
        params.push_back(param);
        param = parameter::gen_param(IR::Direction::Out, "ig_md", "ingress_metadata_t");
        params.push_back(param);
        param = parameter::gen_param(IR::Direction::Out,
                          "ig_intr_md",
                          "ingress_intrinsic_metadata_t");
        params.push_back(param);

        // add to the scope
        for (size_t i = 0; i < params.size(); i++) {
            auto param = params.at(i);
            P4Scope::add_to_scope((IR::Node *)param);

            if (param->direction != IR::Direction::In) {
                P4Scope::add_name_2_type_p(param->name.toString(), param->type);
            }
        }

        tp_parser = new IR::Type_Parser(*p_name, new IR::ParameterList(params));

        // generate decls
        for (int i = 0; i < 5; i++) {
            auto var_decl = variableDeclaration::gen();
            parserLocals.push_back(var_decl);
        }

        // generate states
        auto p4state = new p4State();
        IR::IndexedVector<IR::ParserState> states;
        auto start_state = p4state->gen_start_state();
        // insert custom parsing statements into the start state
        auto pkt_path = new IR::PathExpression("pkt");
        auto pkt_extract = new IR::Member(pkt_path, IR::ID("extract"));
        auto pkt_advance = new IR::Member(pkt_path, IR::ID("advance"));
        auto ig_intr_md = new IR::PathExpression("ig_intr_md");
        auto extract_tofino_md = p4state->gen_hdr_extract(pkt_extract, ig_intr_md);
        start_state->components.push_back(extract_tofino_md);
        auto port_md_size = new IR::PathExpression("PORT_METADATA_SIZE");
        auto advance_tofino_md = p4state->gen_hdr_extract(pkt_advance, port_md_size);
        start_state->components.push_back(advance_tofino_md);
        // done with custom statements
        states.push_back(start_state);
        states.push_back(p4state->gen_hdr_states());

        P4Scope::end_local_scope();

        // add to the whole scope
        auto p4parser = new IR::P4Parser(*p_name,
                                         tp_parser,
                                         parserLocals,
                                         states);
        P4Scope::add_to_scope(p4parser);
        return p4parser;
    }
};
} // namespace CODEGEN


#endif // ifndef _P4PARSER_H_

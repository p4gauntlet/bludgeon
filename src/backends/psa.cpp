#include "psa.h"

#include "actionDeclaration.h"
#include "controlDeclaration.h"
#include "frontends/p4/toP4/toP4.h"
#include "functionDeclaration.h"
#include "headerTypeDeclaration.h"
#include "p4parser.h"
#include "scope.h"
#include "structFieldList.h"
#include "structTypeDeclaration.h"
#include "typeDeclaration.h"

namespace CODEGEN {

void PSA::generate_includes(std::ostream *ostream) {
    *ostream << "#include <core.p4>\n";
    *ostream << "#include <psa.p4>\n\n";
}

static IR::P4Parser *gen_switch_ingress_parser() {
    IR::IndexedVector<IR::Declaration> parserLocals;
    P4Scope::start_local_scope();

    // generate type_parser not that this is labeled "p"
    IR::IndexedVector<IR::Parameter> params;

    params.push_back(
        parameter::gen_param(IR::Direction::None, "pkt", "packet_in"));
    params.push_back(
        parameter::gen_param(IR::Direction::Out, "hdr", SYS_HDR_NAME));
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "user_meta", "metadata_t"));
    params.push_back(parameter::gen_param(
        IR::Direction::In, "istd", "psa_ingress_parser_input_metadata_t"));
    params.push_back(
        parameter::gen_param(IR::Direction::In, "resubmit_meta", "empty_t"));
    params.push_back(
        parameter::gen_param(IR::Direction::In, "recirculate_meta", "empty_t"));
    auto par_list = new IR::ParameterList(params);

    IR::Type_Parser *tp_parser =
        new IR::Type_Parser("IngressParserImpl", par_list);

    // add to the scope
    for (auto param : par_list->parameters) {
        P4Scope::add_to_scope(param);
        // add to the name_2_type
        // only add values that are not read-only to the modifiable types
        if (param->direction == IR::Direction::In) {
            P4Scope::add_lval(param->type, param->name.name, true);
        } else {
            P4Scope::add_lval(param->type, param->name.name, false);
        }
    }

    // // generate decls
    // for (int i = 0; i < 5; i++) {
    //     auto var_decl = variableDeclaration::gen();
    //     parserLocals.push_back(var_decl);
    // }

    // generate states
    IR::IndexedVector<IR::ParserState> states;
    auto start_state = p4State::gen_start_state();
    states.push_back(start_state);
    states.push_back(p4State::gen_hdr_states());

    P4Scope::end_local_scope();

    // add to the whole scope
    auto p4parser =
        new IR::P4Parser("IngressParserImpl", tp_parser, parserLocals, states);
    P4Scope::add_to_scope(p4parser);
    return p4parser;
}

static IR::P4Control *gen_switch_ingress() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "h", SYS_HDR_NAME));
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "user_meta", "metadata_t"));
    params.push_back(parameter::gen_param(IR::Direction::In, "istd",
                                          "psa_ingress_input_metadata_t"));
    params.push_back(parameter::gen_param(IR::Direction::InOut, "ostd",
                                          "psa_ingress_output_metadata_t"));

    auto par_list = new IR::ParameterList(params);
    auto type_ctrl = new IR::Type_Control("ingress", par_list);

    // add to the scope
    for (auto param : par_list->parameters) {
        P4Scope::add_to_scope(param);
        // add to the name_2_type
        // only add values that are not read-only to the modifiable types
        if (param->direction == IR::Direction::In) {
            P4Scope::add_lval(param->type, param->name.name, true);
        } else {
            P4Scope::add_lval(param->type, param->name.name, false);
        }
    }

    IR::IndexedVector<IR::Declaration> local_decls;

    auto blk_stat = controlDeclaration::gen_ctrl_components(local_decls);
    // hardcode the output port to be zero
    /*    auto output_port = new IR::PathExpression("ostd.egress_port");
        auto output_port_val = new IR::Constant(new IR::Type_InfInt(), 0);
        auto assign = new IR::AssignmentStatement(output_port,
       output_port_val);*/
    // some hack to insert the expression at the beginning
    // auto it = blk_stat->components.begin();
    // blk_stat->components.insert(it, assign);
    // end of scope
    P4Scope::end_local_scope();

    // add to the whole scope
    IR::P4Control *p4ctrl =
        new IR::P4Control("ingress", type_ctrl, local_decls, blk_stat);
    P4Scope::add_to_scope(p4ctrl);
    return p4ctrl;
}

static IR::MethodCallStatement *gen_deparser_emit_call() {
    auto call = new IR::PathExpression("pkt.emit");
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();

    args->push_back(new IR::Argument(new IR::PathExpression("h")));

    auto mce = new IR::MethodCallExpression(call, args);
    auto mst = new IR::MethodCallStatement(mce);
    return mst;
}

static IR::P4Control *gen_switch_ingress_deparser() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::None, "pkt", "packet_out"));
    params.push_back(
        parameter::gen_param(IR::Direction::Out, "clone_i2e_meta", "empty_t"));
    params.push_back(
        parameter::gen_param(IR::Direction::Out, "resubmit_meta", "empty_t"));
    params.push_back(
        parameter::gen_param(IR::Direction::Out, "normal_meta", "empty_t"));
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "h", SYS_HDR_NAME));
    params.push_back(
        parameter::gen_param(IR::Direction::In, "user_meta", "metadata_t"));
    params.push_back(parameter::gen_param(IR::Direction::In, "istd",
                                          "psa_ingress_output_metadata_t"));
    auto par_list = new IR::ParameterList(params);
    auto type_ctrl = new IR::Type_Control("IngressDeparserImpl", par_list);
    IR::IndexedVector<IR::Declaration> local_decls;
    auto blk_stat = new IR::BlockStatement();
    blk_stat->push_back(gen_deparser_emit_call());

    return new IR::P4Control("IngressDeparserImpl", type_ctrl, local_decls,
                             blk_stat);
}

static IR::P4Parser *gen_switch_egress_parser() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::None, "pkt", "packet_in"));
    params.push_back(
        parameter::gen_param(IR::Direction::Out, "hdr", SYS_HDR_NAME));
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "user_meta", "metadata_t"));
    params.push_back(parameter::gen_param(
        IR::Direction::In, "istd", "psa_egress_parser_input_metadata_t"));
    params.push_back(
        parameter::gen_param(IR::Direction::In, "normal_meta", "empty_t"));
    params.push_back(
        parameter::gen_param(IR::Direction::In, "clone_i2e_meta", "empty_t"));
    params.push_back(
        parameter::gen_param(IR::Direction::In, "clone_e2e_meta", "empty_t"));
    auto par_list = new IR::ParameterList(params);
    auto type_parser = new IR::Type_Parser("EgressParserImpl", par_list);
    IR::IndexedVector<IR::Declaration> local_decls;
    // TODO: this hacky. FIX
    // generate states
    IR::IndexedVector<IR::ParserState> states;
    IR::IndexedVector<IR::StatOrDecl> components;
    IR::Expression *transition = new IR::PathExpression("accept");
    auto start_state = new IR::ParserState("start", components, transition);
    states.push_back(start_state);
    return new IR::P4Parser("EgressParserImpl", type_parser, local_decls,
                            states);
}

static IR::P4Control *gen_switch_egress() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "h", SYS_HDR_NAME));
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "user_meta", "metadata_t"));
    params.push_back(parameter::gen_param(IR::Direction::In, "istd",
                                          "psa_egress_input_metadata_t"));
    params.push_back(parameter::gen_param(IR::Direction::InOut, "ostd",
                                          "psa_egress_output_metadata_t"));
    auto par_list = new IR::ParameterList(params);
    auto type_ctrl = new IR::Type_Control("egress", par_list);
    IR::IndexedVector<IR::Declaration> local_decls;
    auto blk_stat = new IR::BlockStatement();

    return new IR::P4Control("egress", type_ctrl, local_decls, blk_stat);
}

static IR::P4Control *gen_switch_egress_deparser() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::None, "pkt", "packet_out"));
    params.push_back(
        parameter::gen_param(IR::Direction::Out, "clone_e2e_meta", "empty_t"));
    params.push_back(parameter::gen_param(IR::Direction::Out,
                                          "recirculate_meta", "empty_t"));
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "h", SYS_HDR_NAME));
    params.push_back(
        parameter::gen_param(IR::Direction::In, "user_meta", "metadata_t"));
    params.push_back(parameter::gen_param(IR::Direction::In, "istd",
                                          "psa_egress_output_metadata_t"));
    params.push_back(parameter::gen_param(
        IR::Direction::In, "edstd", "psa_egress_deparser_input_metadata_t"));
    auto par_list = new IR::ParameterList(params);
    auto type_ctrl = new IR::Type_Control("EgressDeparserImpl", par_list);
    IR::IndexedVector<IR::Declaration> local_decls;
    auto blk_stat = new IR::BlockStatement();
    blk_stat->push_back(gen_deparser_emit_call());

    return new IR::P4Control("EgressDeparserImpl", type_ctrl, local_decls,
                             blk_stat);
}

static IR::Declaration_Instance *gen_ingress_pipe_decl() {
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();

    args->push_back(new IR::Argument(new IR::MethodCallExpression(
        new IR::TypeNameExpression("IngressParserImpl"))));
    args->push_back(new IR::Argument(
        new IR::MethodCallExpression(new IR::TypeNameExpression("ingress"))));
    args->push_back(new IR::Argument(new IR::MethodCallExpression(
        new IR::TypeNameExpression("IngressDeparserImpl"))));
    auto package_name = new IR::Type_Name("IngressPipeline");
    return new IR::Declaration_Instance("ip", package_name, args);
}

static IR::Declaration_Instance *gen_egress_pipe_decl() {
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();

    args->push_back(new IR::Argument(new IR::MethodCallExpression(
        new IR::TypeNameExpression("EgressParserImpl"))));
    args->push_back(new IR::Argument(
        new IR::MethodCallExpression(new IR::TypeNameExpression("egress"))));
    args->push_back(new IR::Argument(new IR::MethodCallExpression(
        new IR::TypeNameExpression("EgressDeparserImpl"))));
    auto package_name = new IR::Type_Name("EgressPipeline");
    return new IR::Declaration_Instance("ep", package_name, args);
}

static IR::Declaration_Instance *gen_main() {
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
    args->push_back(new IR::Argument(new IR::TypeNameExpression("ip")));
    args->push_back(new IR::Argument(new IR::MethodCallExpression(
        new IR::TypeNameExpression("PacketReplicationEngine"))));
    args->push_back(new IR::Argument(new IR::TypeNameExpression("ep")));
    args->push_back(new IR::Argument(new IR::MethodCallExpression(
        new IR::TypeNameExpression("BufferingQueueingEngine"))));
    auto package_name = new IR::Type_Name("PSA_Switch");
    return new IR::Declaration_Instance("main", package_name, args);
}

IR::Type_Struct *gen_metadata_t() {
    // Do not emit meta fields for now, no need
    IR::IndexedVector<IR::StructField> fields;
    auto ret = new IR::Type_Struct("metadata_t", fields);
    P4Scope::add_to_scope(ret);
    return ret;
}

IR::Type_Struct *gen_empty_t() {
    // Do not emit meta fields for now, no need
    IR::IndexedVector<IR::StructField> fields;
    auto ret = new IR::Type_Struct("empty_t", fields);
    P4Scope::add_to_scope(ret);
    return ret;
}

void gen_psa_md_t() {
    IR::ID *name;
    IR::Type_Struct *ret;
    IR::IndexedVector<IR::StructField> fields;

    name = new IR::ID("psa_ingress_parser_input_metadata_t");
    ret = new IR::Type_Struct(*name, fields);
    P4Scope::add_to_scope(ret);
    name = new IR::ID("psa_ingress_input_metadata_t");
    ret = new IR::Type_Struct(*name, fields);
    P4Scope::add_to_scope(ret);
    name = new IR::ID("psa_ingress_output_metadata_t");
    ret = new IR::Type_Struct(*name, fields);
    P4Scope::add_to_scope(ret);
    name = new IR::ID("psa_egress_input_metadata_t");
    ret = new IR::Type_Struct(*name, fields);
    P4Scope::add_to_scope(ret);
    name = new IR::ID("psa_egress_output_metadata_t");
    ret = new IR::Type_Struct(*name, fields);
    P4Scope::add_to_scope(ret);
}

void PSA::set_probabilities() {
    PCT.PARAMETER_NONEDIR_DERIVED_STRUCT = 0;
    PCT.PARAMETER_NONEDIR_DERIVED_HEADER = 0;
    PCT.PARAMETER_NONEDIR_BASETYPE_BOOL = 0;
    PCT.PARAMETER_NONEDIR_BASETYPE_ERROR = 0;
    PCT.PARAMETER_NONEDIR_BASETYPE_STRING = 0;
    PCT.PARAMETER_NONEDIR_BASETYPE_VARBIT = 0;
}


IR::P4Program *PSA::gen() {
    // insert banned structures
    P4Scope::not_initialized_structs.insert(
        "psa_ingress_parser_input_metadata_t");
    P4Scope::not_initialized_structs.insert("psa_ingress_input_metadata_t");
    P4Scope::not_initialized_structs.insert("psa_ingress_output_metadata_t");
    P4Scope::not_initialized_structs.insert("psa_egress_input_metadata_t");
    P4Scope::not_initialized_structs.insert("psa_egress_output_metadata_t");
    // set psa-specific probabilities
    set_probabilities();
    // insert some dummy metadata
    gen_psa_md_t();

    // start to assemble the model
    auto objects = new IR::Vector<IR::Node>();

    objects->push_back(headerTypeDeclaration::gen_eth());

    // generate some declarations
    int type_decls = get_rnd_int(DECL.MIN_TYPE, DECL.MAX_TYPE);
    for (int i = 0; i < type_decls; ++i) {
        objects->push_back(typeDeclaration::gen());
    }

    // generate struct Headers
    objects->push_back(structTypeDeclaration::gen_Headers());
    // generate struct metadata_t
    objects->push_back(gen_metadata_t());
    // generate struct empty_t
    objects->push_back(gen_empty_t());

    // generate some callables
    int max_callable_decls =
        get_rnd_int(DECL.MIN_CALLABLES, DECL.MAX_CALLABLES);
    for (int i = 0; i < max_callable_decls; ++i) {
        std::vector<int64_t> percent = {50, 50};
        if (randind(percent)) {
            objects->push_back(actionDeclaration::gen());
        } else {
            // functions do not work in tofino yet
            // objects->push_back(functionDeclaration::gen());
        }
    }

    // generate all the necessary pipelines for the package
    objects->push_back(gen_switch_ingress_parser());
    objects->push_back(gen_switch_ingress());
    objects->push_back(gen_switch_ingress_deparser());
    objects->push_back(gen_switch_egress_parser());
    objects->push_back(gen_switch_egress());
    objects->push_back(gen_switch_egress_deparser());

    // finally assemble the package
    objects->push_back(gen_ingress_pipe_decl());
    objects->push_back(gen_egress_pipe_decl());
    objects->push_back(gen_main());

    return new IR::P4Program(*objects);
}
} // namespace CODEGEN

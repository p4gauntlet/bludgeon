#include "v1model.h"

#include "actionDeclaration.h"
#include "controlDeclaration.h"
#include "frontends/p4/toP4/toP4.h"
#include "functionDeclaration.h"
#include "headerTypeDeclaration.h"
#include "p4parser.h"
#include "scope.h"
#include "structTypeDeclaration.h"
#include "typeDeclaration.h"

namespace CODEGEN {

void V1Model::generate_includes(std::ostream *ostream) {
    *ostream << "#include <core.p4>\n";
    *ostream << "#include <v1model.p4>\n\n";
}

static IR::P4Parser *gen_p() {
    IR::IndexedVector<IR::Declaration> parserLocals;

    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::None, "pkt", "packet_in"));
    params.push_back(
        parameter::gen_param(IR::Direction::Out, "hdr", "Headers"));
    params.push_back(parameter::gen_param(IR::Direction::InOut, "m", "Meta"));
    params.push_back(parameter::gen_param(IR::Direction::InOut, "sm",
                                          "standard_metadata_t"));
    auto par_list = new IR::ParameterList(params);
    IR::Type_Parser *type_parser = new IR::Type_Parser("p", par_list);

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

    // generate decls
    /*        for (int i = 0; i < 5; i++) {
                auto var_decl = variableDeclaration::gen();
                parserLocals.push_back(var_decl);
            }*/

    // generate states
    IR::IndexedVector<IR::ParserState> states;
    states.push_back(p4State::gen_start_state());
    states.push_back(p4State::gen_hdr_states());

    P4Scope::end_local_scope();

    // add to the whole scope
    auto p4parser = new IR::P4Parser("p", type_parser, parserLocals, states);
    P4Scope::add_to_scope(p4parser);
    return p4parser;
}

static IR::P4Control *gen_ingress() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "h", "Headers"));
    params.push_back(parameter::gen_param(IR::Direction::InOut, "m", "Meta"));
    params.push_back(parameter::gen_param(IR::Direction::InOut, "sm",
                                          "standard_metadata_t"));
    auto par_list = new IR::ParameterList(params);
    IR::Type_Control *type_ctrl = new IR::Type_Control("ingress", par_list);

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

    // end of scope
    P4Scope::end_local_scope();

    // add to the whole scope
    IR::P4Control *p4ctrl =
        new IR::P4Control("ingress", type_ctrl, local_decls, blk_stat);
    P4Scope::add_to_scope(p4ctrl);
    P4Scope::p4_ctrls.push_back(p4ctrl);
    return p4ctrl;
}

static IR::P4Control *gen_vrfy() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "h", "Headers"));
    params.push_back(parameter::gen_param(IR::Direction::InOut, "m", "Meta"));
    auto par_list = new IR::ParameterList(params);
    auto type_ctrl = new IR::Type_Control("vrfy", par_list);
    IR::IndexedVector<IR::Declaration> local_decls;
    auto blk_stat = new IR::BlockStatement();

    return new IR::P4Control("vrfy", type_ctrl, local_decls, blk_stat);
}

static IR::P4Control *gen_update() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "h", "Headers"));
    params.push_back(parameter::gen_param(IR::Direction::InOut, "m", "Meta"));
    auto par_list = new IR::ParameterList(params);
    auto type_ctrl = new IR::Type_Control("update", par_list);
    IR::IndexedVector<IR::Declaration> local_decls;
    auto blk_stat = new IR::BlockStatement();

    return new IR::P4Control("update", type_ctrl, local_decls, blk_stat);
}

static IR::P4Control *gen_egress() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "h", "Headers"));
    params.push_back(parameter::gen_param(IR::Direction::InOut, "m", "Meta"));
    params.push_back(parameter::gen_param(IR::Direction::InOut, "sm",
                                          "standard_metadata_t"));
    auto par_list = new IR::ParameterList(params);
    auto type_ctrl = new IR::Type_Control("egress", par_list);
    IR::IndexedVector<IR::Declaration> local_decls;
    auto blk_stat = new IR::BlockStatement();

    return new IR::P4Control("egress", type_ctrl, local_decls, blk_stat);
}

static IR::MethodCallStatement *gen_deparser_emit_call() {
    auto call = new IR::PathExpression("pkt.emit");
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();

    args->push_back(new IR::Argument(new IR::PathExpression("h")));

    auto mce = new IR::MethodCallExpression(call, args);
    auto mst = new IR::MethodCallStatement(mce);
    return mst;
}

static IR::P4Control *gen_deparser() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::None, "pkt", "packet_out"));
    params.push_back(parameter::gen_param(IR::Direction::In, "h", "Headers"));
    auto par_list = new IR::ParameterList(params);
    auto type_ctrl = new IR::Type_Control("deparser", par_list);
    IR::IndexedVector<IR::Declaration> local_decls;
    auto blk_stat = new IR::BlockStatement();
    blk_stat->push_back(gen_deparser_emit_call());

    return new IR::P4Control("deparser", type_ctrl, local_decls, blk_stat);
}

static IR::Declaration_Instance *gen_main() {
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();

    args->push_back(new IR::Argument(
        new IR::MethodCallExpression(new IR::TypeNameExpression("p"))));
    args->push_back(new IR::Argument(
        new IR::MethodCallExpression(new IR::TypeNameExpression("vrfy"))));
    args->push_back(new IR::Argument(
        new IR::MethodCallExpression(new IR::TypeNameExpression("ingress"))));
    args->push_back(new IR::Argument(
        new IR::MethodCallExpression(new IR::TypeNameExpression("egress"))));
    args->push_back(new IR::Argument(
        new IR::MethodCallExpression(new IR::TypeNameExpression("update"))));
    args->push_back(new IR::Argument(
        new IR::MethodCallExpression(new IR::TypeNameExpression("deparser"))));
    auto package_name = new IR::Type_Name("V1Switch");
    return new IR::Declaration_Instance("main", package_name, args);
}

IR::Type_Struct *gen_meta() {
    // Do not emit meta fields for now, no need
    // FIXME: Design a way to emit these that plays nicely with all targets
    // auto   sfl   = new structFieldList(STRUCT, name->name);
    // IR::IndexedVector< IR::StructField > fields = sfl->gen(rand()%5+1);
    IR::IndexedVector<IR::StructField> fields;

    auto ret = new IR::Type_Struct("Meta", fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

IR::Type_Struct *gen_standard_metadata_t() {
    auto fields = structFieldList::gen_sm();

    auto ret = new IR::Type_Struct("standard_metadata_t", fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

IR::P4Program *V1Model::gen() {
    // insert banned structures
    P4Scope::not_initialized_structs.insert("standard_metadata_t");

    // start to assemble the model
    auto objects = new IR::Vector<IR::Node>();

    objects->push_back(headerTypeDeclaration::gen_eth());

    // generate some declarations
    int max_type_decls =
        MIN_T_DECLS + (rand() % (MAX_T_DECLS - MIN_T_DECLS + 1));
    for (int i = 0; i < max_type_decls; ++i) {
        objects->push_back(typeDeclaration::gen());
    }

    // generate struct Headers
    objects->push_back(structTypeDeclaration::gen_Headers());

    // generate struct Meta
    objects->push_back(gen_meta());
    // insert standard_metadata_t
    gen_standard_metadata_t();

    // generate some callables
    int max_callable_decls =
        MIN_CALLABLES + (rand() % (MAX_CALLABLES - MIN_CALLABLES + 1));
    for (int i = 0; i < max_callable_decls; ++i) {
        std::vector<int> percent = {50, 50};
        if (randind(percent)) {
            objects->push_back(actionDeclaration::gen());
        } else {
            objects->push_back(functionDeclaration::gen());
        }
    }

    // generate all the necessary pipelines for the package
    objects->push_back(gen_p());
    objects->push_back(gen_ingress());
    objects->push_back(gen_vrfy());
    objects->push_back(gen_update());
    objects->push_back(gen_egress());
    objects->push_back(gen_deparser());

    // finally assemble the package
    objects->push_back(gen_main());

    return new IR::P4Program(*objects);
}
} // namespace CODEGEN

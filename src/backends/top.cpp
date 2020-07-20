#include "top.h"

#include "frontends/p4/toP4/toP4.h"

#include "bludgeon/src/actionDeclaration.h"
#include "bludgeon/src/controlDeclaration.h"
#include "bludgeon/src/externDeclaration.h"
#include "bludgeon/src/functionDeclaration.h"
#include "bludgeon/src/headerTypeDeclaration.h"
#include "bludgeon/src/p4parser.h"
#include "bludgeon/src/scope.h"
#include "bludgeon/src/structFieldList.h"
#include "bludgeon/src/structTypeDeclaration.h"
#include "bludgeon/src/typeDeclaration.h"

namespace CODEGEN {

void Top::generate_includes(std::ostream *ostream) {
    *ostream << "#include <core.p4>\n";
}

IR::P4Parser *Top::gen_p() {
    IR::IndexedVector<IR::Declaration> parserLocals;

    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::None, "pkt", "packet_in"));
    params.push_back(
        parameter::gen_param(IR::Direction::Out, "hdr", SYS_HDR_NAME));
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

IR::P4Control *Top::gen_ingress() {
    // start of new scope
    P4Scope::start_local_scope();

    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "h", SYS_HDR_NAME));
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
    return p4ctrl;
}

IR::Declaration_Instance *Top::gen_main() {
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();

    args->push_back(new IR::Argument(
        new IR::MethodCallExpression(new IR::TypeNameExpression("p"))));
    args->push_back(new IR::Argument(
        new IR::MethodCallExpression(new IR::TypeNameExpression("ingress"))));
    auto package_name = new IR::Type_Name("top");
    return new IR::Declaration_Instance("main", package_name, args);
}

IR::Type_Parser *Top::gen_parser_type() {
    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::None, "b", "packet_in"));
    params.push_back(
        parameter::gen_param(IR::Direction::Out, "hdr", SYS_HDR_NAME));
    auto par_list = new IR::ParameterList(params);
    return new IR::Type_Parser("Parser", par_list);
}

IR::Type_Control *Top::gen_ingress_type() {
    IR::IndexedVector<IR::Parameter> params;
    params.push_back(
        parameter::gen_param(IR::Direction::InOut, "hdr", SYS_HDR_NAME));
    auto par_list = new IR::ParameterList(params);
    return new IR::Type_Control("Ingress", par_list);
}

IR::Type_Package *Top::gen_package() {
    IR::IndexedVector<IR::Parameter> params;
    params.push_back(parameter::gen_param(IR::Direction::None, "p", "Parser"));
    params.push_back(
        parameter::gen_param(IR::Direction::None, "ig", "Ingress"));
    auto par_list = new IR::ParameterList(params);
    return new IR::Type_Package("top", par_list);
}

IR::P4Program *Top::gen() {
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

    // generate some callables
    int callable_decls = get_rnd_int(DECL.MIN_CALLABLES, DECL.MAX_CALLABLES);
    for (int i = 0; i < callable_decls; ++i) {
        std::vector<int64_t> percent = {70, 20, 10};
        switch (randind(percent)) {
        case 0: {
            objects->push_back(functionDeclaration::gen());
            break;
        }
        case 1: {
            objects->push_back(actionDeclaration::gen());
            break;
        }
        case 2: {
            objects->push_back(externDeclaration::gen());
            break;
        }
        }
    }

    // generate all the necessary pipelines for the package
    objects->push_back(gen_p());
    objects->push_back(gen_ingress());

    // finally assemble the package
    objects->push_back(gen_parser_type());
    objects->push_back(gen_ingress_type());
    objects->push_back(gen_package());
    objects->push_back(gen_main());

    return new IR::P4Program(*objects);
}
} // namespace CODEGEN

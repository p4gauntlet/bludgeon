#include "controlDeclaration.h"

namespace CODEGEN {
IR::BlockStatement *controlDeclaration::gen_ctrl_components(
    IR::IndexedVector<IR::Declaration>& local_decls) {

    auto max_vars    = rand() % MAX_VAR_DECL;
    auto max_decls   = rand() % MAX_DECL_INS_DECL;
    auto max_actions = rand() % MAX_ACTION_DECL;
    auto max_tables  = rand() % MAX_TABLE_DECL;

    // variableDeclarations
    for (int i = 0; i <= max_vars; i++) {
        auto var_decl = variableDeclaration::gen();
        local_decls.push_back(var_decl);
    }

    // declaration_instance
    for (int i = 0; i <= max_decls; i++) {
        auto decl_ins = gen_decl_instance();

        if (decl_ins == nullptr) {
            continue;
        }
        local_decls.push_back(decl_ins);
    }

    // actionDeclarations
    for (int i = 0; i <= max_actions; i++) {
        auto act_decl = new actionDeclaration();
        local_decls.push_back(act_decl->gen());
    }


    for (int i = 0; i <= max_tables; i++) {
        auto tab_decl = tableDeclaration::gen();
        local_decls.push_back(tab_decl);
    }

    // instantiations

    // blockstatement
    auto blk_gen  = new blockStatement();
    auto blk_stat = blk_gen->gen();

    return blk_stat;
}


IR::P4Control *controlDeclaration::gen_ing_ctrl() {
    // start of new scope
    P4Scope::start_local_scope();

    //
    IR::Type_Control *type_ctrl = controlTypeDeclaration::gen_ing_ctrl_type(
        0);

    // IR::ParameterList* constructor_params;
    IR::IndexedVector<IR::Declaration> local_decls;

    auto blk_stat = controlDeclaration::gen_ctrl_components(local_decls);

    // end of scope
    P4Scope::end_local_scope();

    // add to the whole scope
    IR::P4Control *p4ctrl = new IR::P4Control("ingress",
                                              type_ctrl,
                                              local_decls,
                                              blk_stat);
    P4Scope::add_to_scope(p4ctrl);
    P4Scope::p4_ctrls.push_back(p4ctrl);
    return p4ctrl;
}


IR::P4Control *controlDeclaration::gen_tf_ing_ctrl() {
    // start of new scope
    P4Scope::start_local_scope();

    //
    IR::Type_Control *type_ctrl = controlTypeDeclaration::gen_ing_ctrl_type(
        1);

    IR::IndexedVector<IR::Declaration> local_decls;

    auto blk_stat = gen_ctrl_components(local_decls);
    // hardcode the output port to be zero
    auto output_port = new IR::PathExpression(
        "ig_tm_md.ucast_egress_port");
    auto output_port_val = new IR::Constant(int_literal::gen(), 0);
    auto assign          = new IR::AssignmentStatement(output_port,
                                                       output_port_val);
    // some hack to insert the expression at the beginning
    auto it = blk_stat->components.begin();
    blk_stat->components.insert(it, assign);
    // end of scope
    P4Scope::end_local_scope();

    // add to the whole scope
    IR::P4Control *p4ctrl = new IR::P4Control("ingress",
                                              type_ctrl,
                                              local_decls,
                                              blk_stat);
    P4Scope::add_to_scope(p4ctrl);
    P4Scope::p4_ctrls.push_back(p4ctrl);
    return p4ctrl;
}


IR::Declaration_Instance *controlDeclaration::gen_decl_instance() {
    size_t size = P4Scope::p4_ctrls.size();

    if (size == 0) {
        return nullptr;
    }
    IR::Declaration_Instance *decl_ins = nullptr;
    IR::Vector<IR::Argument> *args     = new IR::Vector<IR::Argument>();
    IR::P4Control *p4ctrl = P4Scope::p4_ctrls.at(rand() % size);
    IR::Type *tp          = new IR::Type_Name(p4ctrl->name);

    cstring name = randstr(6);
    decl_ins = new IR::Declaration_Instance(name, tp, args);
    P4Scope::decl_ins_ctrls.emplace(name, p4ctrl);

    return decl_ins;
}
} // namespace CODEGEN

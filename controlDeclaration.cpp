#include "controlDeclaration.h"

namespace CODEGEN {
IR::BlockStatement *controlDeclaration::gen_ctrl_components(
    IR::IndexedVector<IR::Declaration> &local_decls) {

    auto max_vars = rand() % MAX_VAR_DECL;
    auto max_decls = rand() % MAX_DECL_INS_DECL;
    auto max_actions = rand() % MAX_ACTION_DECL;
    auto max_tables = rand() % MAX_TABLE_DECL;

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
        auto act_decl = actionDeclaration::gen();
        local_decls.push_back(act_decl);
    }

    for (int i = 0; i <= max_tables; i++) {
        auto tab_decl = tableDeclaration::gen();
        local_decls.push_back(tab_decl);
    }

    // instantiations

    // blockstatement
    auto blk_stat = blockStatement::gen();

    return blk_stat;
}

IR::Declaration_Instance *controlDeclaration::gen_decl_instance() {
    auto p4_ctrls = P4Scope::get_decls<IR::P4Control>();
    size_t size = p4_ctrls.size();

    if (size == 0) {
        //FIXME: Figure out a better way to handle this nullptr
        return nullptr;
    }
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
    const IR::P4Control *p4ctrl = p4_ctrls.at(rand() % size);
    IR::Type *tp = new IR::Type_Name(p4ctrl->name);

    return new IR::Declaration_Instance(randstr(6), tp, args);
}
} // namespace CODEGEN

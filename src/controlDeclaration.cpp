#include "controlDeclaration.h"

namespace CODEGEN {
IR::BlockStatement *controlDeclaration::gen_ctrl_components(
    IR::IndexedVector<IR::Declaration> &local_decls) {

    auto vars = get_rnd_int(DECL.MIN_VAR, DECL.MAX_VAR);
    auto decls = get_rnd_int(DECL.MIN_INSTANCE, DECL.MAX_INSTANCE);
    auto actions = get_rnd_int(DECL.MIN_ACTION, DECL.MAX_ACTION);
    auto tables = get_rnd_int(DECL.MIN_TABLE, DECL.MAX_TABLE);

    // variableDeclarations
    for (int i = 0; i <= vars; i++) {
        auto var_decl = variableDeclaration::gen();
        local_decls.push_back(var_decl);
    }

    // declaration_instance
    for (int i = 0; i <= decls; i++) {
        auto decl_ins = gen_decl_instance();

        if (decl_ins == nullptr) {
            continue;
        }
        local_decls.push_back(decl_ins);
    }

    // actionDeclarations
    for (int i = 0; i <= actions; i++) {
        auto act_decl = actionDeclaration::gen();
        local_decls.push_back(act_decl);
    }

    for (int i = 0; i <= tables; i++) {
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
        // FIXME: Figure out a better way to handle this nullptr
        return nullptr;
    }
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
    const IR::P4Control *p4ctrl = p4_ctrls.at(get_rnd_int(0, size - 1));
    IR::Type *tp = new IR::Type_Name(p4ctrl->name);

    return new IR::Declaration_Instance(randstr(6), tp, args);
}
} // namespace CODEGEN

#include "functionDeclaration.h"

#include "baseType.h"
#include "blockStatement.h"
#include "expression.h"
#include "parameterList.h"
#include "scope.h"

namespace CODEGEN {

IR::Type *gen_return_type() {
    std::vector<int64_t> percent = {90, 9, 1};
    IR::Type *tp = nullptr;
    switch (randind(percent)) {
    case 0: {
        std::vector<int> b_types = {0, 1};
        tp = baseType::pick_rnd_base_type(b_types);
        break;
    }
    case 1: {
        auto l_types = P4Scope::get_decls<IR::Type_StructLike>();
        if (l_types.size() == 0) {
            return nullptr;
        }
        auto candidate_type = l_types.at(rand() % l_types.size());
        auto type_name = candidate_type->name.name;
        // check if struct is forbidden
        if (P4Scope::not_initialized_structs.count(type_name) == 0) {
            tp = new IR::Type_Name(candidate_type->name.name);
        } else {
            std::vector<int> b_types = {0, 1};
            tp = baseType::pick_rnd_base_type(b_types);
        }
        break;
    }
    case 2: {
        // return a void type
        tp = new IR::Type_Void();
        break;
    }
    }
    return tp;
}

IR::Function *functionDeclaration::gen() {
    cstring name = randstr(7);
    IR::Type_Method *tm = nullptr;
    IR::BlockStatement *blk = nullptr;
    P4Scope::start_local_scope();
    IR::ParameterList *params = parameterList::gen();

    IR::Type *r_tp = gen_return_type();
    tm = new IR::Type_Method(r_tp, params);

    P4Scope::prop.ret_type = r_tp;
    blk = blockStatement::gen(true);
    P4Scope::prop.ret_type = nullptr;

    auto ret = new IR::Function(name, tm, blk);
    P4Scope::end_local_scope();
    P4Scope::add_to_scope(ret);
    return ret;
}

} // namespace CODEGEN

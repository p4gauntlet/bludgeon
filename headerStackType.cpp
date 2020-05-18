#include "headerStackType.h"

namespace CODEGEN {
IR::Type *headerStackType::gen() {

    auto l_types = P4Scope::get_decls<IR::Type_Header>();
    if (l_types.size() < 1) {
        BUG("Creating a header stacks assumes at least one declared header!");
    }
    auto hdr_tp = l_types.at(rand() % l_types.size());
    auto stack_size = rand() % MAX_HEADER_STACK_SIZE + 1;
    auto hdr_type_name = new IR::Type_Name(hdr_tp->name);
    auto ret = new IR::Type_Stack(hdr_type_name, new IR::Constant(stack_size));

    P4Scope::add_to_scope(ret);

    return ret;
}
} // namespace CODEGEN

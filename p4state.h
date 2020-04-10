#ifndef _P4STATE_H_
#define _P4STATE_H_

#include "ir/ir.h"

#include "assignmentOrMethodCallStatement.h"
#include "common.h"
#include "expression.h"
#include "scope.h"
#include "variableDeclaration.h"

namespace CODEGEN {
class p4State {
  public:
    p4State() {}

    ~p4State() {}

    static IR::MethodCallStatement *gen_hdr_extract(IR::Member *pkt_call,
                                                    IR::Expression *mem);
    static void
    gen_hdr_union_extract(IR::IndexedVector<IR::StatOrDecl> &components,
                          const IR::Type_HeaderUnion *hdru,
                          IR::ArrayIndex *arr_ind, IR::Member *pkt_call);

    static IR::ParserState *gen_start_state();

    static IR::ParserState *gen_hdr_states();

}; // class p4State
} // namespace CODEGEN

#endif

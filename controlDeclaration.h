#ifndef _CONTROLDECLARATION_H_
#define _CONTROLDECLARATION_H_

#include "ir/ir.h"


#include "scope.h"
#include "expression.h"
#include "controlTypeDeclaration.h"
#include "constantDeclaration.h"
#include "variableDeclaration.h"
#include "actionDeclaration.h"
#include "tableDeclaration.h"
#include "blockStatement.h"


#define MAX_VAR_DECL         5
#define MAX_DECL_INS_DECL    5
#define MAX_ACTION_DECL      2
#define MAX_TABLE_DECL       1

namespace CODEGEN {
class controlDeclaration {
public:

    const char *types[0] = {
    };


    controlDeclaration() {
    }

    ~controlDeclaration() {
    }

    static IR::BlockStatement *gen_ctrl_components(
        IR::IndexedVector<IR::Declaration>& local_decls);
    static IR::P4Control *gen_ing_ctrl();

    static IR::P4Control *gen_tf_ing_ctrl();

    static IR::Declaration_Instance *gen_decl_instance();
};
} // namespace CODEGEN


#endif // ifndef _CONTROLDECLARATION_H_

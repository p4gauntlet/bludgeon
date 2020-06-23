#ifndef _CONTROLDECLARATION_H_
#define _CONTROLDECLARATION_H_

#include "ir/ir.h"

#include "actionDeclaration.h"
#include "blockStatement.h"
#include "constantDeclaration.h"
#include "controlTypeDeclaration.h"
#include "expression.h"
#include "scope.h"
#include "tableDeclaration.h"
#include "variableDeclaration.h"

namespace CODEGEN {
class controlDeclaration {
  public:
    const char *types[0] = {};

    controlDeclaration() {}

    ~controlDeclaration() {}

    static IR::BlockStatement *
    gen_ctrl_components(IR::IndexedVector<IR::Declaration> &local_decls);

    static IR::Declaration_Instance *gen_decl_instance();
};
} // namespace CODEGEN

#endif // ifndef _CONTROLDECLARATION_H_

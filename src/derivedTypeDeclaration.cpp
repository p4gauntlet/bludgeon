#include "derivedTypeDeclaration.h"
#include "headerTypeDeclaration.h"

namespace CODEGEN {

IR::Type *derivedTypeDeclaration::gen() { return headerTypeDeclaration::gen(); }

} // namespace CODEGEN

#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include <cstdlib>
#include <cstring>
#include <string>

#include "ir/ir.h"
#include "lib/nullstream.h"

#include "scope.h"

namespace CODEGEN {
class CGenerator {
  public:
    CGenerator() {}
    ~CGenerator() {}
};
} // namespace CODEGEN

#endif // _CODEGEN_H_

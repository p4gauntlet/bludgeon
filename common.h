#ifndef _COMMON_H_
#define _COMMON_H_

#define HEADER "header"
#define HEADER_UNION "header_union"
#define STRUCT "struct"
#define STRUCT_HEADERS "struct_headers"
#define STRUCT_LIKE "struct_like"
#define HEADER_ONLY "HEADER_ONLY"

#include "ir/ir.h"

namespace CODEGEN {

cstring randstr(size_t len);

int randind(std::vector<int> &percent, size_t n_num);

} // namespace CODEGEN



#endif

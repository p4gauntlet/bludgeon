#ifndef _COMMON_H_
#define _COMMON_H_

#define HEADER "header"
#define HEADER_UNION "header_union"
#define STRUCT "struct"
#define STRUCT_HEADERS "struct_headers"
#define STRUCT_LIKE "struct_like"
#define HEADER_ONLY "HEADER_ONLY"

#define ETH_HEADER_T "ethernet_t"
#define ETH_HDR "eth_hdr"

#include "ir/ir.h"

namespace CODEGEN {

cstring randstr(size_t len);

int randind(std::vector<int> &percent);

} // namespace CODEGEN



#endif

#ifndef _COMMON_H_
#define _COMMON_H_

#define ETH_HEADER_T "ethernet_t"
#define ETH_HDR "eth_hdr"

#include "ir/ir.h"

namespace CODEGEN {

cstring randstr(size_t len);

int randind(std::vector<int> &percent);

} // namespace CODEGEN

#endif

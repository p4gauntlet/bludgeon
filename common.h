#ifndef _COMMON_H_
#define _COMMON_H_

#define ETH_HEADER_T "ethernet_t"
#define ETH_HDR "eth_hdr"

#include "ir/ir.h"

// minimum and maximum number of type declarations
#define MIN_T_DECLS 1
#define MAX_T_DECLS 8

// minimum and maximum number of statements in a block statement
#define MAX_STAT 10
#define MIN_STAT 3

// minimum and maximum number of callable declarations
#define MIN_CALLABLES 0
#define MAX_CALLABLES 4

namespace CODEGEN {

cstring randstr(size_t len);
int randind(const std::vector<int> &percent);

} // namespace CODEGEN

#endif

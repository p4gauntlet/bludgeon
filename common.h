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
int64_t randind(const std::vector<int64_t> &percent);
int64_t get_rnd_int(int64_t min, int64_t max);

} // namespace CODEGEN

#endif

#ifndef _COMMON_H_
#define _COMMON_H_

#include "ir/ir.h"
#include "probabilities.h"

#define INTEGER_WIDTH 32

// hardcoded initialization names
#define SYS_HDR_NAME "Headers"
#define ETH_HEADER_T "ethernet_t"
#define ETH_HDR "eth_hdr"

namespace CODEGEN {

cstring randstr(size_t len);
void set_seed(int64_t seed);
int64_t randind(const std::vector<int64_t> &percent);
int64_t get_rnd_int(int64_t min, int64_t max);
big_int get_rnd_big_int(big_int min, big_int max);

} // namespace CODEGEN

#endif

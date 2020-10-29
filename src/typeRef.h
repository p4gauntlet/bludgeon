#ifndef _TYPEREF_H_
#define _TYPEREF_H_

#include "ir/ir.h"

#include "common.h"

namespace CODEGEN {
typedef struct typeref_probs {
    int64_t p4_bit;
    int64_t p4_signed_bit;
    int64_t p4_varbit;
    int64_t p4_int;
    int64_t p4_error;
    int64_t p4_bool;
    int64_t p4_string;
    // derived types
    int64_t p4_enum;
    int64_t p4_header;
    int64_t p4_header_stack;
    int64_t p4_struct;
    int64_t p4_header_union;
    int64_t p4_tuple;
    int64_t p4_void;
    int64_t p4_match_kind;
} typeref_probs;

class typeRef {
 public:
    const char *types[5] = {
        "baseType", "typeName",
        "specializedType", // not
        "headerStackType",
        "tupleType" // not
    };

    typeRef() {}
    ~typeRef() {}
    static IR::Type *pick_rnd_type(typeref_probs);
};
} // namespace CODEGEN

#endif

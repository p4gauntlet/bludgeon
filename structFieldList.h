#ifndef _STRUCTFIELDLIST_H_
#define _STRUCTFIELDLIST_H_

#include <set>
#include <vector>

#include "ir/ir.h"

#include "structField.h"

namespace CODEGEN {
class structFieldList {
  public:
    const char *types[0] = {};

    structFieldList() {}

    static IR::IndexedVector<IR::StructField> gen(cstring for_type,
                                                  cstring name, size_t len);
    static IR::IndexedVector<IR::StructField> gen_sm();

    static IR::IndexedVector<IR::StructField> gen_tf_ing_md_t();
    static IR::IndexedVector<IR::StructField> gen_tf_ing_md_for_tm_t();
    static IR::IndexedVector<IR::StructField> gen_tf_ing_intr_md_from_prsr();
    static IR::IndexedVector<IR::StructField> gen_tf_ing_intr_md_for_deprsr();
    static IR::IndexedVector<IR::StructField> gen_tf_eg_intr_md_t();
    static IR::IndexedVector<IR::StructField> gen_tf_eg_intr_md_from_prsr();
    static IR::IndexedVector<IR::StructField> gen_tf_eg_intr_md_for_deprsr();
    static IR::IndexedVector<IR::StructField>
    gen_tf_eg_intr_md_for_output_port();
};
} // namespace CODEGEN

#endif // ifndef _STRUCTFIELDLIST_H_

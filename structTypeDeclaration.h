#ifndef _STRUCTTYPEDECLARATION_H_
#define _STRUCTTYPEDECLARATION_H_

#include "ir/ir.h"

#include "annotations.h"
#include "common.h"
#include "scope.h"
#include "structFieldList.h"

namespace CODEGEN {
class structTypeDeclaration {
  public:
    const char *types[0] = {};

    structTypeDeclaration() {}

    static IR::Type_Struct *gen() {
        cstring name = randstr(6);
        IR::IndexedVector<IR::StructField> fields =
            structFieldList::gen(STRUCT, name, rand() % 5 + 1);

        auto ret = new IR::Type_Struct(name, fields);

        P4Scope::add_to_scope(ret);

        return ret;
    }

    static IR::Type_Struct *gen_Headers() {
        IR::IndexedVector<IR::StructField> fields =
            structFieldList::gen(STRUCT_HEADERS, "Headers", rand() % 5 + 1);

        // Tao: hard code for ethernet_t eth_hdr;
        auto eth_sf =
            new IR::StructField(ETH_HDR, new IR::Type_Name(ETH_HEADER_T));

        fields.insert(fields.begin(), eth_sf);

        auto ret = new IR::Type_Struct("Headers", fields);

        P4Scope::sys_hdr = ret;
        P4Scope::add_to_scope(ret);

        return ret;
    }

    static IR::Type_Struct *gen_Meta() {
        IR::ID *name = new IR::ID("Meta");

        // Do not emit meta fields for now, no need
        // FIXME: Design a way to emit these that plays nicely with all targets
        // auto   sfl   = new structFieldList(STRUCT, name->name);
        // IR::IndexedVector< IR::StructField > fields = sfl->gen(rand()%5+1);
        IR::IndexedVector<IR::StructField> fields;

        auto ret = new IR::Type_Struct(*name, fields);

        P4Scope::add_to_scope(ret);

        return ret;
    }

    static IR::Type_Struct *gen_Sm() {
        auto fields = structFieldList::gen_sm();

        auto ret = new IR::Type_Struct("standard_metadata_t", fields);

        P4Scope::add_to_scope(ret);

        return ret;
    }

    static void gen_tf_md_t() {
        IR::ID *name;
        // IR::IndexedVector<IR::StructField> fields;
        IR::Type_Struct *ret;

        name = new IR::ID("ingress_intrinsic_metadata_t");
        ret = new IR::Type_Struct(*name, structFieldList::gen_tf_ing_md_t());
        P4Scope::add_to_scope(ret);
        name = new IR::ID("ingress_intrinsic_metadata_for_tm_t");
        ret = new IR::Type_Struct(*name,
                                  structFieldList::gen_tf_ing_md_for_tm_t());
        P4Scope::add_to_scope(ret);
        name = new IR::ID("ingress_intrinsic_metadata_from_parser_t");
        ret = new IR::Type_Struct(
            *name, structFieldList::gen_tf_ing_intr_md_from_prsr());
        P4Scope::add_to_scope(ret);
        name = new IR::ID("ingress_intrinsic_metadata_for_deparser_t");
        ret = new IR::Type_Struct(
            *name, structFieldList::gen_tf_ing_intr_md_for_deprsr());
        P4Scope::add_to_scope(ret);
        name = new IR::ID("egress_intrinsic_metadata_t");
        ret =
            new IR::Type_Struct(*name, structFieldList::gen_tf_eg_intr_md_t());
        P4Scope::add_to_scope(ret);
        name = new IR::ID("egress_intrinsic_metadata_from_parser_t");
        ret = new IR::Type_Struct(
            *name, structFieldList::gen_tf_eg_intr_md_from_prsr());
        P4Scope::add_to_scope(ret);
        name = new IR::ID("egress_intrinsic_metadata_for_deparser_t");
        ret = new IR::Type_Struct(
            *name, structFieldList::gen_tf_eg_intr_md_for_deprsr());
        P4Scope::add_to_scope(ret);
        name = new IR::ID("egress_intrinsic_metadata_for_output_port_t");
        ret = new IR::Type_Struct(
            *name, structFieldList::gen_tf_eg_intr_md_for_output_port());
        P4Scope::add_to_scope(ret);
    }
};
} // namespace CODEGEN

#endif // ifndef _STRUCTTYPEDECLARATION_H_

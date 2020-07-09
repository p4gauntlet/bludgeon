#include "headerTypeDeclaration.h"

#include "baseType.h"
#include "common.h"
#include "scope.h"
#include "structFieldList.h"
#include "typeRef.h"

namespace CODEGEN {

IR::Type_Header *headerTypeDeclaration::gen() {
    cstring name = randstr(6);
    IR::IndexedVector<IR::StructField> fields;
    typeref_probs type_percent = {
        PCT.HEADERTYPEDECLARATION_BASETYPE_BIT,
        PCT.HEADERTYPEDECLARATION_BASETYPE_SIGNED_BIT,
        PCT.HEADERTYPEDECLARATION_BASETYPE_VARBIT,
        PCT.HEADERTYPEDECLARATION_BASETYPE_INT,
        PCT.HEADERTYPEDECLARATION_BASETYPE_ERROR,
        PCT.HEADERTYPEDECLARATION_BASETYPE_BOOL,
        PCT.HEADERTYPEDECLARATION_BASETYPE_STRING,
        PCT.HEADERTYPEDECLARATION_DERIVED_ENUM,
        PCT.HEADERTYPEDECLARATION_DERIVED_HEADER,
        PCT.HEADERTYPEDECLARATION_DERIVED_HEADER_STACK,
        PCT.HEADERTYPEDECLARATION_DERIVED_STRUCT,
        PCT.HEADERTYPEDECLARATION_DERIVED_HEADER_UNION,
        PCT.HEADERTYPEDECLARATION_DERIVED_TUPLE,
        PCT.HEADERTYPEDECLARATION_TYPE_VOID,
        PCT.HEADERTYPEDECLARATION_TYPE_MATCH_KIND,
    };

    size_t len = get_rnd_int(1, 5);
    for (size_t i = 0; i < len; i++) {
        cstring field_name = randstr(4);
        IR::Type *field_tp = typeRef::pick_rnd_type(type_percent);

        if (auto struct_tp = field_tp->to<IR::Type_Struct>()) {
            field_tp = new IR::Type_Name(struct_tp->name);
        }
        IR::StructField *sf = new IR::StructField(field_name, field_tp);
        fields.push_back(sf);
    }
    auto ret = new IR::Type_Header(name, fields);
    if (P4Scope::req.byte_align_headers) {
        auto remainder = ret->width_bits() % 8;
        if (remainder) {
            auto pad_bit = new IR::Type_Bits(8 - remainder, false);
            auto pad_field = new IR::StructField("padding", pad_bit);
            ret->fields.push_back(pad_field);
        }
    }

    printf("%d\n", ret->width_bits());

    P4Scope::add_to_scope(ret);

    return ret;
}

IR::Type_Header *headerTypeDeclaration::gen_eth() {
    IR::IndexedVector<IR::StructField> fields;
    auto eth_dst =
        new IR::StructField("dst_addr", new IR::Type_Bits(48, false));
    auto eth_src =
        new IR::StructField("src_addr", new IR::Type_Bits(48, false));
    auto eth_type =
        new IR::StructField("eth_type", new IR::Type_Bits(16, false));

    fields.push_back(eth_dst);
    fields.push_back(eth_src);
    fields.push_back(eth_type);

    auto ret = new IR::Type_Header(IR::ID(ETH_HEADER_T), fields);
    P4Scope::add_to_scope(ret);

    return ret;
}

} // namespace CODEGEN

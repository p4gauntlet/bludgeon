#ifndef _STRUCTFIELDLIST_H_
#define _STRUCTFIELDLIST_H_

#include <set>
#include <vector>

#include "ir/ir.h"

#include "structField.h"


namespace CODEGEN {
class structFieldList {
public:

    const char *types[0] = {
    };

    cstring for_type;
    cstring name;
    std::set<cstring>fields_name;
    std::set<cstring>fields_type;

    structFieldList(cstring for_type, cstring name) :
        for_type(for_type), name(name) {}

    IR::IndexedVector<IR::StructField>gen(size_t len) {
        bool if_contain_stack = false;
        IR::IndexedVector<IR::StructField> fields;

        for (size_t i = 0; i < len; i++) {
            auto struct_field   = new structField(for_type);
            IR::StructField *sf = struct_field->gen();

            if (sf == nullptr) {
                continue;
            }

            if (sf->type->is<IR::Type_Stack>() ||
                sf->type->is<IR::Type_Header>()
                || sf->type->is<IR::Type_HeaderUnion>()) {
                if_contain_stack = true;
            }

            // we check the filed name and type here
            if (fields_name.find(sf->name.name) != fields_name.end()) {
                delete struct_field;
                delete sf;
                continue;
            }

            cstring hdr_tpn;

            if (for_type == HEADER_UNION) {
                auto tpn = sf->type->to<IR::Type_Name>();
                hdr_tpn = tpn->path->name.name;

                if (fields_type.find(hdr_tpn) != fields_type.end()) {
                    delete struct_field;
                    delete sf;
                    continue;
                }
            }

            fields_name.insert(sf->name.name);
            fields_type.insert(hdr_tpn);

            fields.push_back(sf);
        }

        if (if_contain_stack == true) {
            P4Scope::insert_type_name(name);
        }

        return fields;
    }

    static IR::IndexedVector<IR::StructField>gen_sm() {
        IR::IndexedVector<IR::StructField> fields;

        IR::ID   *name;
        IR::Type *tp;

        /*
           name = new IR::ID("ingress_port");
           tp = new IR::Type_Bits(9, false);
           fields.push_back(new IR::StructField(*name, tp));
           name = new IR::ID("egress_spec");
           tp = new IR::Type_Bits(9, false);
           fields.push_back(new IR::StructField(*name, tp));
           name = new IR::ID("egress_port");
           tp = new IR::Type_Bits(9, false);
           fields.push_back(new IR::StructField(*name, tp));
           name = new IR::ID("instance_type");
           tp = new IR::Type_Bits(32, false);
           fields.push_back(new IR::StructField(*name, tp));
           name = new IR::ID("packet_length");
           tp = new IR::Type_Bits(32, false);
           fields.push_back(new IR::StructField(*name, tp));
           name = new IR::ID("enq_timestamp");
           tp = new IR::Type_Bits(32, false);
           fields.push_back(new IR::StructField(*name, tp));
           name = new IR::ID("enq_qdepth");
           tp = new IR::Type_Bits(19, false);
           fields.push_back(new IR::StructField(*name, tp));
           // name = new IR::ID("dep_timedelta");
           // tp = new IR::Type_Bits(32, false);
           // fields.push_back(new IR::StructField(*name, tp));
           name = new IR::ID("deq_qdepth");
           tp = new IR::Type_Bits(19, false);
           fields.push_back(new IR::StructField(*name, tp));
           name = new IR::ID("ingress_global_timestamp");
           tp = new IR::Type_Bits(48, false);
           fields.push_back(new IR::StructField(*name, tp));
           name = new IR::ID("egress_global_timestamp");
           tp = new IR::Type_Bits(48, false);
           fields.push_back(new IR::StructField(*name, tp));
           name = new IR::ID("egress_rid");
           tp = new IR::Type_Bits(16, false);
           fields.push_back(new IR::StructField(*name, tp));
           // Tao: error is omitted here
           name = new IR::ID("priority");
           tp = new IR::Type_Bits(3, false);
           fields.push_back(new IR::StructField(*name, tp));
         */

        return fields;
    }

    static IR::IndexedVector<IR::StructField>gen_tf_ing_md_t();
    static IR::IndexedVector<IR::StructField>gen_tf_ing_md_for_tm_t();
    static IR::IndexedVector<IR::StructField>gen_tf_ing_intr_md_from_prsr();
    static IR::IndexedVector<IR::StructField>gen_tf_ing_intr_md_for_deprsr();
    static IR::IndexedVector<IR::StructField>gen_tf_eg_intr_md_t();
    static IR::IndexedVector<IR::StructField>gen_tf_eg_intr_md_from_prsr();
    static IR::IndexedVector<IR::StructField>gen_tf_eg_intr_md_for_deprsr();
    static IR::IndexedVector<IR::StructField>gen_tf_eg_intr_md_for_output_port();
};
} // namespace CODEGEN


#endif // ifndef _STRUCTFIELDLIST_H_

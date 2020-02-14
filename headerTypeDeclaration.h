#ifndef _HEADERTYPEDECLARATION_H_
#define _HEADERTYPEDECLARATION_H_

#include "ir/ir.h"

#include "scope.h"
#include "structFieldList.h"

#include "common.h"

namespace CODEGEN {


class headerTypeDeclaration {
public:
	const char* types[0] = {
	};

	headerTypeDeclaration() {
	}

	static IR::Type_Header* gen() {

		IR::ID *name;
		name = new IR::ID(CODEGEN::randstr(6));
		auto sfl = new structFieldList(HEADER, name->name);
		IR::IndexedVector< IR::StructField > fields = sfl->gen(rand()%5+1);
		auto ret = new IR::Type_Header(*name, fields);

		P4Scope::add_to_scope(ret);


		return ret;
	}

    static IR::Type_Header* gen_eth() {
        IR::IndexedVector< IR::StructField > fields;
        auto eth_dst = new IR::StructField(IR::ID("dst_addr"), new IR::Type_Bits(48 ,false));
        auto eth_src = new IR::StructField(IR::ID("src_addr"), new IR::Type_Bits(48 ,false));
        auto eth_type = new IR::StructField(IR::ID("eth_type"), new IR::Type_Bits(16 ,false));

        fields.push_back(eth_dst);
        fields.push_back(eth_src);
        fields.push_back(eth_type);

        auto ret = new IR::Type_Header(IR::ID(ETH_HEADER_T), fields);
		P4Scope::add_to_scope(ret);

        return ret;
    }
};


} // namespace CODEGEN




#endif

#ifndef _STRUCTFIELDLIST_H_
#define _STRUCTFIELDLIST_H_

#include <set>

#include "ir/ir.h"

#include "structField.h"



namespace CODEGEN {


class structFieldList {
public:
	const char* types[0] = {
	};

	cstring for_type;
    cstring name;
	std::set<cstring> fields_name;
	std::set<cstring> fields_type;

	structFieldList(cstring for_type, cstring name) :
                for_type(for_type), name(name) {
	}

	IR::IndexedVector< IR::StructField > gen(size_t len) {
        bool if_contain_stack = false;
		IR::IndexedVector< IR::StructField > fields;
		for (size_t i=0; i<len; i++) {
			auto struct_field = new structField(for_type);
			IR::StructField* sf = struct_field->gen();
			if (sf == nullptr) {
				continue;
			}
			
            if (sf->type->is<IR::Type_Stack>() || sf->type->is<IR::Type_Header>()
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
            std::cout << name << std::endl;
            P4Scope::insert_type_name(name);
        }

		return fields;
	}

};


} // namespace CODEGEN




#endif

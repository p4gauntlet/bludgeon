#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#include "ir/ir.h"

#include "codegen.h"
#include "baseType.h"

namespace CODEGEN {


class parameter {
public:
	const char* types[0] = {
	};


	bool if_none_dir = false;
	IR::ID *name;
	IR::Type *tp;
	IR::Direction dir;
	

	parameter(bool if_none_dir) :
		if_none_dir(if_none_dir) {
		name = new IR::ID(CODEGEN::randstr(4));
		if (if_none_dir == false) {
			switch (rand()%3) {
				// case 0: dir = IR::Direction::None; break;
				case 0: dir = IR::Direction::In; break;
				case 1: dir = IR::Direction::Out; break;
				case 2: dir = IR::Direction::InOut; break;
			}
		}
		else {
			dir = IR::Direction::None;
		}
	}
	~parameter() {
		delete name;
		delete tp;
	}

	// Tao: the parameter seems can only be int<> bit<> in bmv2 model
	IR::Parameter* gen() {
		std::vector<int> type;
		type.push_back(1);
		type.push_back(2);
		type.push_back(3);
		auto base_type = new baseType(true, type);
		tp = base_type->gen();

		return new IR::Parameter(*name, dir, tp);
	}

	static IR::Parameter* gen_ing_headers() {
		IR::ID hdr("h");
		IR::Direction hdr_dir = IR::Direction::InOut;

		IR::ID tp_name("Headers");
		IR::Type* tp = new IR::Type_Name(new IR::Path(tp_name));

		return new IR::Parameter(hdr, hdr_dir, tp);
	}

	static IR::Parameter* gen_ing_meta() {
		IR::ID meta("m");
		IR::Direction meta_dir = IR::Direction::InOut;

		IR::ID tp_name("Meta");
		IR::Type* tp = new IR::Type_Name(new IR::Path(tp_name));

		return new IR::Parameter(meta, meta_dir, tp);
	}

	static IR::Parameter* gen_ing_sm() {
		IR::ID sm("sm");
		IR::Direction sm_dir = IR::Direction::InOut;

		IR::ID tp_name("standard_metadata_t");
		IR::Type* tp = new IR::Type_Name(new IR::Path(tp_name));

		return new IR::Parameter(sm, sm_dir, tp);
	}
};


} // namespace CODEGEN




#endif

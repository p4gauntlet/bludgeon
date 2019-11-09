#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include <cstring>
#include <string>
#include <cstdlib>

#include "ir/ir.h"

#include "scope.h"

static const char alphanum[] =
	// "0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz";


namespace CODEGEN {

cstring randstr(size_t len);



class CGenerator {
public:
	CGenerator() {
		P4Scope::start_local_scope();
	}

	~CGenerator() {
		P4Scope::end_local_scope();
	}



	IR::Node* gen();

};




} // namespace CODEGEN







#endif // _CODEGEN_H_

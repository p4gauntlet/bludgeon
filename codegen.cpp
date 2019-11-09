#include "codegen.h"


#include "scope.h"
#include "headerTypeDeclaration.h"
#include "headerStackType.h"
#include "headerUnionDeclaration.h"

cstring CODEGEN::randstr(size_t len) {
	std::stringstream ss;
	for (size_t i=0; i<len; i++) {
		ss << alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return ss.str();
}

namespace CODEGEN {


IR::Node* CGenerator::gen() {
	IR::Node* n = nullptr;
 	while (1) {
		switch (rand()%3) {
			case 0: {
						n = headerTypeDeclaration::gen();
						break;
					}
			case 1: {
						if (P4Scope::get_num_type_header() > 0) {
							n = headerUnionDeclaration::gen(); 
						}
						break;
					}
		}

		if (n != nullptr) {
			break;
		}
	}

	return n;
}


} // namespace CODEGEN

#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include <cstring>
#include <string>
#include <cstdlib>

#include "ir/ir.h"
#include "lib/nullstream.h"

#include "scope.h"
#include "sub_toP4.h"

static const char alphanum[] =

// "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";


namespace CODEGEN {
class CGenerator {
public:

    cstring o_file;
    std::ostream *ostream = nullptr;
    int flag              = -1;

    CGenerator(char *filename, int flag) :
        o_file(filename), flag(flag) {
        ostream = openFile(o_file, false);
        P4Scope::start_local_scope();
    }

    ~CGenerator() {
        P4Scope::end_local_scope();
    }

    static void emitBmv2Top(std::ostream *ostream);
    static void emitBmv2Bottom(std::ostream *ostream);
    static void emitTFTop(std::ostream *ostream);
    static void emitTFBottom(std::ostream *ostream);

    // =====================
    IR::Node  * gen();
    IR::Node  * gen_act();
    IR::Node  * gen_struct();
    IR::Node  * gen_t_enum();
    IR::Node  * gen_tpdef();
    IR::Node  * gen_ctrldef();
    IR::Node  * gen_actlist();
    IR::Node  * gen_tab();
    IR::Node  * gen_func();
    IR::Node  * gen_sys_parser();


    void        gen_p4_code();
};
} // namespace CODEGEN


#endif // _CODEGEN_H_

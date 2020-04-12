#include <cstdlib>
#include <cstring>
#include <string>

#include "common.h"
#include "scope.h"

const std::vector<cstring> str_keywords = {"if", "else", "key", "actions"};

static const char alphanum[] =

    // "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

namespace CODEGEN {
cstring randstr(size_t len) {
    cstring ret;
    std::stringstream ss;

    while (1) {
        ss.str("");
        for (size_t i = 0; i < len; i++) {
            ss << alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        ret = ss.str();
        if (std::find(str_keywords.begin(), str_keywords.end(), ret) !=
            str_keywords.end()) {
            continue;
        }

        if (P4Scope::used_names.find(ret) == P4Scope::used_names.end()) {
            P4Scope::used_names.insert(ret);
            break;
        }
    }

    return ret;
}

int randind(std::vector<int> &percent) {

    int sum = 0;
    for (auto i : percent) {
        sum += i;
    }

    int rand_num = rand() % sum;
    int ret = 0;

    int ret_sum = 0;
    for (auto i : percent) {
        ret_sum += i;
        if (ret_sum > rand_num) {
            break;
        }
        ret = ret + 1;
    }

    return ret;
}
} // namespace CODEGEN

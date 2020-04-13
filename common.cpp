#include <cstdlib>
#include <cstring>
#include <random>
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

        // name is usable, break the loop
        if (P4Scope::used_names.count(ret) == 0) {
            break;
        }
        delete ret;
    }

    P4Scope::used_names.insert(ret);
    return ret;
}

int randind(const std::vector<int> &percent) {
    int sum = accumulate(percent.begin(), percent.end(), 0);

    // Will be used to obtain a seed for the random number engine
    std::random_device rd;
    // Standard mersenne_twister_engine seeded with rd()
    std::mt19937 gen(rd());
    // do not pick zero since that conflicts with zero percentage values
    std::uniform_int_distribution<> dis(1, sum);

    int rand_num = dis(rd);
    int ret = 0;

    int ret_sum = 0;
    for (auto i : percent) {
        ret_sum += i;
        if (ret_sum >= rand_num) {
            break;
        }
        ret = ret + 1;
    }
    return ret;
}
} // namespace CODEGEN

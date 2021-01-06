#include <cstdlib>
#include <cstring>
#include <random>
#include <string>

#include <boost/random.hpp>

#include "common.h"
#include "scope.h"

const std::vector<cstring> str_keywords = {"if",  "void",    "else",
                                           "key", "actions", "true"};

static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";
// "0123456789"

namespace CODEGEN {
static boost::random::mt19937 rng;

void set_seed(int64_t seed) { rng = boost::mt19937(seed); }

int64_t get_rnd_int(int64_t min, int64_t max) {
    boost::random::uniform_int_distribution<int64_t> distribution(min, max);
    return distribution(rng);
}

big_int get_rnd_big_int(big_int min, big_int max) {
    boost::random::uniform_int_distribution<big_int> distribution(min, max);
    return distribution(rng);
}

cstring randstr(size_t len) {
    cstring ret;
    std::stringstream ss;

    while (1) {
        ss.str("");
        for (size_t i = 0; i < len; i++) {
            ss << alphanum[get_rnd_int(0, sizeof(alphanum) - 2)];
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

int64_t randind(const std::vector<int64_t> &percent) {
    int sum = accumulate(percent.begin(), percent.end(), 0);

    // do not pick zero since that conflicts with zero percentage values
    auto rand_num = get_rnd_int(1, sum);
    int ret = 0;

    int64_t ret_sum = 0;
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

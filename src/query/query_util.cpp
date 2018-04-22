#include <query_util.h>

#include <random>

namespace dpt {
namespace query {

namespace {

static const char ALPHANUM[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

}

std::string QueryUtil::prependRandomDomain(const std::string& domain,
                                           int                len) {
    std::string result;
    for (int i = 0; i < len; ++i) {
        result += ALPHANUM[rand() % (sizeof(ALPHANUM) - 1)];
    }
    result += '.';
    result.append(domain);
    return result;
}

}
}
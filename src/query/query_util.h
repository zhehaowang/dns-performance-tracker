#ifndef INCLUDED_QUERY_UTIL
#define INCLUDED_QUERY_UTIL

#include <cstdint>
#include <cmath>
#include <string>

namespace dpt {
namespace query {

struct QueryUtil {
    static double getAverage(uint64_t total, uint64_t count);
        // Calculate the average given 'total' and 'count'

    static double newVariance(double variance,
                              double avg,
                              double total,
                              double count,
                              double sample);
        // Calculate the new population variance given existing 'variance',
        // 'avg', 'count', 'total' and the new 'sample' using Welford method

    static double getStdDeviation(double variance);
        // Get standard deviation given 'variance'

    static std::string prependRandomDomain(const std::string& domain, int len);
        // Return a string that consists of a random string of length 'len'
        // prepended to given 'domain'
};


inline
double QueryUtil::getAverage(uint64_t total, uint64_t count) {
    return static_cast<double>(total) / count;
}

inline
double QueryUtil::getStdDeviation(double variance) {
    return sqrt(variance);
}
inline
double QueryUtil::newVariance(double variance,
                              double avg,
                              double total,
                              double count,
                              double sample) {
    double newAvg = (total + sample) / (count + 1);
    return (count * variance + (sample - avg) * (sample - newAvg)) / (count + 1);
}

}
}

#endif
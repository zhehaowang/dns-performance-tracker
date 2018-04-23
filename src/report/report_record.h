#ifndef INCLUDED_REPORT_RECORD
#define INCLUDED_REPORT_RECORD

#include <cstdint>
#include <string>

namespace dpt {
namespace report {

class Record {
  public:
    Record(const std::string& domain,
           uint64_t           duration,
           bool               isSuccess);
        // Construct a report record from 'domain', 'duration' and 'isSuccess'.
        // 'duration' is considered meaningful only if 'isSuccess' is true.

    const std::string& domain() const;
    std::uint64_t duration() const;
    bool isSuccess() const;
  private:
    std::string d_domain;
  	uint64_t    d_duration;
    bool        d_isSuccess;
};

inline
Record::Record(const std::string& domain,
               uint64_t           duration,
               bool               isSuccess)
  : d_domain(domain),
    d_duration(duration),
    d_isSuccess(isSuccess)
{}

inline
const std::string& Record::domain() const {
  return d_domain;
}

inline
uint64_t Record::duration() const {
    return d_duration;
}

inline
bool Record::isSuccess() const { return d_isSuccess; }

}
}

#endif
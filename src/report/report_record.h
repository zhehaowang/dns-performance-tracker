#ifndef INCLUDED_REPORT_RECORD
#define INCLUDED_REPORT_RECORD

#include <chrono>
#include <cstdint>

namespace dpt {
namespace report {

class Record {
  public:
    Record(const std::chrono::milliseconds& duration,
    	   const std::chrono::milliseconds& timestamp);

    std::uint64_t durationAsUInt64() const;
    std::uint64_t timestampAsUInt64() const;
  private:
  	std::chrono::milliseconds d_duration;
  	std::chrono::milliseconds d_timestamp;
};

inline
Record::Record(const std::chrono::milliseconds& duration,
    	   	   const std::chrono::milliseconds& timestamp)
 : d_duration(duration), d_timestamp(timestamp)
{}

inline
std::uint64_t Record::durationAsUInt64() const {
    return d_duration.count();
}

inline
std::uint64_t Record::timestampAsUInt64() const {
    return d_timestamp.count();
}

}
}

#endif
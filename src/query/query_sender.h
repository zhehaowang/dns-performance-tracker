#ifndef INCLUDED_QUERY_SENDER
#define INCLUDED_QUERY_SENDER

#include <report_reporterinterface.h>

#include <string>

namespace dpt {
namespace query {

class QuerySender {
  public:
    QuerySender(report::ReporterInterface *reporter);
    int sendQuery(const std::string& dest);
  private:
  	report::ReporterInterface *p_reporter;
};

inline
QuerySender::QuerySender(report::ReporterInterface *reporter)
 : p_reporter(reporter)
{};

}
} 

#endif
#ifndef INCLUDED_QUERY_SENDER
#define INCLUDED_QUERY_SENDER

#include <report_reporterinterface.h>

#include <string>
#include <memory>

namespace dpt {
namespace query {

class QuerySender {
  public:
    QuerySender(std::shared_ptr<report::ReporterInterface> reporter);

    int sendQuery(const std::string& dest);
  private:
  	std::shared_ptr<report::ReporterInterface> p_reporter;
};

inline
QuerySender::QuerySender(std::shared_ptr<report::ReporterInterface> reporter)
 : p_reporter(reporter)
{};

}
}

#endif
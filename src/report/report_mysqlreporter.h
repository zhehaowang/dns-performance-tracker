#ifndef INCLUDED_REPORT_MYSQLREPORTER
#define INCLUDED_REPORT_MYSQLREPORTER

#include <report_reporterinterface.h>

namespace dpt {
namespace report {

class MySQLReporter : public ReporterInterface {
  public:
  	MySQLReporter();

  	virtual int reportRecord(const Record& record);
  private:

};

inline
MySQLReporter::MySQLReporter()
{}

}
}

#endif
#ifndef INCLUDED_REPORT_MYSQLREPORTER
#define INCLUDED_REPORT_MYSQLREPORTER

#include <report_reporterinterface.h>

#include <mysql++/mysql++.h>

namespace dpt {
namespace report {

class MySQLReporter : public ReporterInterface {
  public:
  	MySQLReporter(const char* dbName,
  				  const char* server,
  				  const char* user,
  				  const char* pass);

  	virtual int reportRecord(const Record& record);
  private:
  	mysqlpp::Connection d_connection;
};

}
}

#endif
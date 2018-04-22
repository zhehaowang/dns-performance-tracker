#ifndef INCLUDED_REPORT_MYSQLREPORTER
#define INCLUDED_REPORT_MYSQLREPORTER

#include <report_reporterinterface.h>

#include <report_dbrecord.h>

#include <mysql++/mysql++.h>

#include <string>
#include <vector>

namespace dpt {
namespace report {

class MySQLReporter : public ReporterInterface {
  public:
    MySQLReporter(const char* dbName,
                  const char* server,
                  const char* user,
                  const char* pass);
        // Construct MySQL reporter for 'dbName' by 'user'@'server' identified
        // by 'pass'

    virtual int reportRecord(const Record& record);

  private:
    int readStatsRecordByDomain(std::vector<DBRecord> *dbRecords,
                                const std::string&     domain);
        // Read in a stats entry identified by 'domain' from this DB connection
        // object.  Populates 'dbRecords' with its content.  Returns 0 for
        // success, otherwise failure
    int upsertStatsRecordByDomain(const DBRecord& dbRecord,
                                  bool            isInsert);
        // Writes a stats entry 'dbRecord' identified by its to this DB
        // connection object if 'isInsert', otherwise update the entry with the
        // same domain name.  Returns 0 for success, otherwise failure
  
    mysqlpp::Connection d_connection;
};

}
}

#endif
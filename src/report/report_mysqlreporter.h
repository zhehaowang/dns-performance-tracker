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
                  const char* pass,
                  bool        journalQuery = true);
        // Construct MySQL reporter for 'dbName' by 'user'@'server' identified
        // by 'pass'.  Optionally set 'journalQuery' to determine if each
        // report call will have the result logged to journal table as well

    virtual int reportRecord(const Record& record);
        // Report given 'record' to stats table and if 'journalQuery' is set,
        // journal table as well

  private:
    int readStatsRecordByDomain(std::vector<StatRecord> *dbRecords,
                                const std::string&       domain);
        // Read in a stats entry identified by 'domain' from this DB connection
        // object.  Populates 'dbRecords' with its content.  Returns 0 for
        // success, otherwise failure
    int upsertStatsRecordByDomain(const StatRecord& dbRecord,
                                  bool              isInsert);
        // Writes a stats entry 'dbRecord' identified by its to this DB
        // connection object if 'isInsert', otherwise update the entry with the
        // same domain name.  Returns 0 for success, otherwise failure
    int writeQueryRecord(QueryRecord *queryRecord);
        // Writes the 'queryRecord'.  Return 0 for success, otherwise failure
  
    mysqlpp::Connection d_connection;
    bool                d_journalQuery;
};

}
}

#endif
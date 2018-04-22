#include <report_mysqlreporter.h>

#include <sstream>
#include <stdexcept>

namespace dpt {
namespace report {

MySQLReporter::MySQLReporter(const char* dbName,
                             const char* server,
                             const char* user,
                             const char* pass)
  : d_connection(false) {
    if (!d_connection.connect(dbName, server, user, pass)) {
        std::stringstream ss;
        ss << "Failed to connect to DB '"
           << dbName << "' '"
           << user << "'@'"
           << server << "\n";
        throw std::runtime_error(ss.str());
    }
}

int MySQLReporter::reportRecord(const Record& record) {
    return 0;
}

}
}
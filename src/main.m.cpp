#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <cassert>
#include <chrono>
#include <thread>
#include <getopt.h>

#include <query_sender.h>
#include <report_mysqlreporter.h>

#define LOGURU_IMPLEMENTATION 1
#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

struct DBConnectionInfo {
    std::string dbName;
    std::string host;
    std::string user;
    std::string passwd;
};

const int DB_CONF_SIZE = 4;

int getDomains(std::vector<std::string> *domains,
               const std::string&        confFileName) {
    assert(domains);

    std::ifstream in(confFileName);
    std::string   line;

    while (std::getline(in, line)) {
        domains->push_back(line);
    }

    return 0;
}

int getDBConnectionInfo(DBConnectionInfo   *conn,
                        const std::string&  confFileName) {
    assert(conn);

    std::ifstream in(confFileName);
    std::string   line;
    int           size = 0;
    while (std::getline(in, line) && size < DB_CONF_SIZE) {
        switch (size) {
          case 0:
            conn->dbName = line;
            break;
          case 1:
            conn->host = line;
            break;
          case 2:
            conn->user = line;
            break;
          case 3:
            conn->passwd = line;
            break;
        }
        size += 1;
    }
    return size != DB_CONF_SIZE;
}

void usage() {
    std::cout << "./dns_performance_tracker\n"
              << "    [-h]: print this message\n"
              << "    [-i]: specify query interval (milliseconds)"
              << "\n"
              << "    [-l]: specify log file\n"
              << "    [-d]: specify DB configuration file\n"
              << "        (format: four consecutive lines of db name,"
              << " host name, username, passwd)\n"
              << "    [-s]: specify domains to query (one line"
              << " each domain)\n"
              << "    [-v]: console logging verbosity\n";
}

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace dpt;
    

    int         interval = 2000;
    string dbConfFile("db.conf");
    string siteConfFile("sites.conf");
    string logFile("");

    loguru::init(argc, argv);

    int c = 0;
    while (c >= 0) {
        c = getopt(argc, argv, "i:d:s:l:h");
      
        if (c < 0) {
            break;
        }
        switch (c) {
          case 'i':
            interval = atoi(optarg);
            break;
          case 'd':
            dbConfFile = optarg;
            break;
          case 's':
            siteConfFile = optarg;
            break;
          case 'l':
            logFile = optarg;
            break;
          case 'h':
            usage();
            return 0;
          default:
            usage();
            return 0;
        }
    }

    if (logFile != "") {
        loguru::add_file(logFile.c_str(),
                         loguru::Append,
                         loguru::Verbosity_MAX);
    }

    srand(time(NULL));
    // Initialize Reporter and QuerySender
    vector<string> domains;
    if (getDomains(&domains, siteConfFile) || domains.size() == 0) {
        LOG_S(ERROR) << "Failed to load domains from configuration file '"
                     << siteConfFile << "'\n";
        return -1;
    }

    DBConnectionInfo conn;
    if (getDBConnectionInfo(&conn, dbConfFile)) {
        LOG_S(ERROR) << "Failed to load db connection configuration file '"
                     << dbConfFile << "'\n";
        return -1;
    }

    shared_ptr<report::MySQLReporter> reporter =
        std::make_shared<report::MySQLReporter>(conn.dbName.c_str(),
                                                conn.host.c_str(),
                                                conn.user.c_str(),
                                                conn.passwd.c_str());
    query::QuerySender query(reporter);

    // Query loop
    while (true) {
        LOG_S(INFO) << "Beginning next batch of queries. Sites: "
                    << domains.size() << ". Interval: " << interval << "ms.";
        for (auto it = domains.cbegin(); it != domains.cend(); ++it) {
            query.sendQuery(*it);
        }
        this_thread::sleep_for(std::chrono::milliseconds(interval));
    }

    return 0;
}
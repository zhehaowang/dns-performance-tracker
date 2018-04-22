#define MYSQLPP_SSQLS_NO_STATICS

#include <report_mysqlreporter.h>

#include <query_util.h>

#include <sstream>
#include <stdexcept>
#include <cstdint>
#include <cassert>
#include <iomanip>

namespace dpt {
namespace report {

// TODO: add logging

MySQLReporter::MySQLReporter(const char* dbName,
                             const char* server,
                             const char* user,
                             const char* pass)
  : d_connection(false) {
    assert(dbName);
    assert(server);
    assert(user);
    assert(pass);

    if (!d_connection.connect(dbName, server, user, pass)) {
        std::stringstream ss;
        ss << "Failed to connect to DB '"
           << dbName << "' '"
           << user << "'@'"
           << server << "\n";
        throw std::runtime_error(ss.str());
    }
}

int MySQLReporter::readStatsRecordByDomain(std::vector<DBRecord>* dbRecords,
                                           const std::string&     domain) {
    assert(dbRecords);
    try {
        mysqlpp::Query query = d_connection.query();
        // mysqlpp query stream operator takes care of escaping and quoting
        query << "select * from domain_stats where domain = "
              << mysqlpp::quote << domain;
        std::cout << query << "\n";
        query.storein(*dbRecords);
        if (d_connection.errnum()) {
            std::cout << d_connection.error() << "\n";
            return -1;
        }
    } catch (const mysqlpp::BadQuery& er) {
        // Handle any query errors
        std::cerr << "Query error: " << er.what() << std::endl;
        return -1;
    } catch (const mysqlpp::BadConversion& er) {
        // Handle bad conversions; e.g. type mismatch populating 'stock'
        std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        return -1;
    } catch (const mysqlpp::Exception& ex) {
        // Catch-all for any other MySQL++ exceptions
        std::cerr << ex.what() << "\n";
        return -1;
    }
    
    return 0;

}

int MySQLReporter::upsertStatsRecordByDomain(const DBRecord& dbRecord,
                                             bool            isInsert) {
    try {
        mysqlpp::Query query = d_connection.query();

        if (isInsert) {
            query.insert(dbRecord);
        } else {
            DBRecord oldRecord;
            oldRecord.domain = dbRecord.domain;

            query.update(oldRecord, dbRecord);
        }

        std::cout << "Query: " << query << std::endl;

        query.execute();
        if (d_connection.errnum()) {
            std::cout << d_connection.error() << "\n";
            return -1;
        }
    } catch (const mysqlpp::BadQuery& er) {
        // Handle any query errors
        std::cerr << "Query error: " << er.what() << std::endl;
        return -1;
    } catch (const mysqlpp::BadConversion& er) {
        // Handle bad conversions; e.g. type mismatch populating 'stock'
        std::cerr << "Conversion error: " << er.what() << std::endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << std::endl;
        return -1;
    } catch (const mysqlpp::Exception& ex) {
        // Catch-all for any other MySQL++ exceptions
        std::cerr << ex.what() << "\n";
        return -1;
    }

    return 0;
}

int MySQLReporter::reportRecord(const Record& record) {
    std::vector<report::DBRecord> res;
    readStatsRecordByDomain(&res, record.domain());

    if (res.size()) {
        report::DBRecord& dbr = res[0];
        dbr.last_time = mysqlpp::sql_timestamp::now();
        if (record.isSuccess()) {

            std::cout << "***\n" << dbr.variance << "\n" << dbr.number_success
                      << "\n" << dbr.total_duration << "***\n";
            dbr.variance = query::QueryUtil::newVariance(
                                dbr.variance,
                                dbr.total_duration / dbr.number_success,
                                dbr.total_duration,
                                dbr.number_success,
                                record.duration());
            dbr.number_success += 1;
            dbr.total_duration += record.duration();
            upsertStatsRecordByDomain(dbr, false);
        } else {
            dbr.number_fail += 1;
            upsertStatsRecordByDomain(dbr, false);
        }
    } else {
        auto now = mysqlpp::sql_timestamp::now();
        report::DBRecord dbr(record.domain(),
                             0,
                             record.isSuccess() ? 1 : 0,
                             record.isSuccess() ? 0 : 1,
                             record.isSuccess() ? record.duration() : 0,
                             now,
                             now);
        upsertStatsRecordByDomain(dbr, true);
    }

    return 0;
}

}
}
#define MYSQLPP_SSQLS_NO_STATICS

#include <report_mysqlreporter.h>
#include <query_util.h>

#include <sstream>
#include <stdexcept>
#include <cstdint>
#include <cassert>
#include <iomanip>

#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

namespace dpt {
namespace report {

MySQLReporter::MySQLReporter(const char* dbName,
                             const char* server,
                             const char* user,
                             const char* pass,
                             bool        journalQuery)
  : d_connection(false), d_journalQuery(journalQuery) {

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
        LOG_S(FATAL) << ss.str();
    }
}

int MySQLReporter::readStatsRecordByDomain(std::vector<StatRecord>* dbRecords,
                                           const std::string&       domain) {
    assert(dbRecords);
    try {
        mysqlpp::Query query = d_connection.query();
        // mysqlpp query stream operator takes care of escaping and quoting
        query << "select * from domain_stats where domain = "
              << mysqlpp::quote << domain;
        LOG_S(TRACE) << "Query: " << query.str();
        query.storein(*dbRecords);
        if (d_connection.errnum()) {
            LOG_S(WARNING) << d_connection.error();
            return -1;                                               // RETURN
        }
    } catch (const mysqlpp::BadQuery& ex) {
        // Handle any query errors
        LOG_S(WARNING) << "Query error: " << ex.what();
        return -1;                                                   // RETURN
    } catch (const mysqlpp::BadConversion& ex) {
        // Handle bad conversions; e.g. type mismatch populating 'stock'
        LOG_S(WARNING) << "Conversion error: " << ex.what();
        return -1;                                                   // RETURN
    } catch (const mysqlpp::Exception& ex) {
        // Catch-all for any other MySQL++ exceptions
        LOG_S(WARNING) << ex.what();
        return -1;                                                   // RETURN
    }
    
    return 0;

}

int MySQLReporter::upsertStatsRecordByDomain(const StatRecord& dbRecord,
                                             bool              isInsert) {
    try {
        mysqlpp::Query query = d_connection.query();

        if (isInsert) {
            query.insert(dbRecord);
        } else {
            StatRecord oldRecord;
            oldRecord.domain = dbRecord.domain;

            query.update(oldRecord, dbRecord);
        }

        LOG_S(TRACE) << "Query: " << query.str();

        query.execute();
        if (d_connection.errnum()) {
            LOG_S(WARNING) << d_connection.error();
            return -1;                                               // RETURN
        }
    } catch (const mysqlpp::BadQuery& ex) {
        // Handle any query errors
        LOG_S(WARNING) << "Query error: " << ex.what();
        return -1;                                                   // RETURN
    } catch (const mysqlpp::BadConversion& ex) {
        // Handle bad conversions; e.g. type mismatch populating 'stock'
        LOG_S(WARNING) << "Conversion error: " << ex.what();
        return -1;                                                   // RETURN
    } catch (const mysqlpp::Exception& ex) {
        // Catch-all for any other MySQL++ exceptions
        LOG_S(WARNING) << ex.what();
        return -1;                                                   // RETURN
    }

    return 0;
}

int MySQLReporter::writeQueryRecord(QueryRecord *queryRecord) {
    try {
        mysqlpp::Query query = d_connection.query();
        queryRecord->id = query.insert_id();

        query.insert(*queryRecord);
        LOG_S(TRACE) << "Query: " << query.str();

        query.execute();
        if (d_connection.errnum()) {
            LOG_S(WARNING) << d_connection.error();
            return -1;                                               // RETURN
        }
    } catch (const mysqlpp::BadQuery& ex) {
        // Handle any query errors
        LOG_S(WARNING) << "Query error: " << ex.what();
        return -1;                                                   // RETURN
    } catch (const mysqlpp::BadConversion& ex) {
        // Handle bad conversions; e.g. type mismatch populating 'stock'
        LOG_S(WARNING) << "Conversion error: " << ex.what();
        return -1;                                                   // RETURN
    } catch (const mysqlpp::Exception& ex) {
        // Catch-all for any other MySQL++ exceptions
        LOG_S(WARNING) << ex.what();
        return -1;                                                   // RETURN
    }

    return 0;
}


int MySQLReporter::reportRecord(const Record& record) {
    std::vector<report::StatRecord> stats;
    int ret = readStatsRecordByDomain(&stats, record.domain());
    if (ret) {
        return ret;                                                   // RETURN
    }

    auto now = mysqlpp::sql_timestamp::now();
    // Update stats of the domain in record
    if (stats.size()) {
        StatRecord& stat = stats[0];
        auto lastQueryTime = stat.last_time;
        stat.last_time = now;
        if (record.isSuccess()) {
            stat.variance = query::QueryUtil::newVariance(
                                stat.variance,
                                static_cast<double>(stat.total_duration)
                                    / stat.number_success,
                                stat.total_duration,
                                stat.number_success,
                                record.duration());
            stat.number_success += 1;
            stat.total_duration += record.duration();
            ret = upsertStatsRecordByDomain(stat, false);
        } else {
            stat.number_fail += 1;
            ret = upsertStatsRecordByDomain(stat, false);
        }

        LOG_S(INFO) << "\n=== Domain " << record.domain() << " ===\n"
                    << "  Average query time (ms):       "
                    << static_cast<double>(stat.total_duration)
                        / stat.number_success << "\n"
                    << "  Std Deviation query time (ms): "
                    << query::QueryUtil::getStdDeviation(stat.variance) << "\n"
                    << "  Total # of queries:            "
                    << stat.number_success + stat.number_fail << "\n"
                    << "  Timestamp of first query:      "
                    << stat.first_time << "\n"
                    << "  Timestamp of last query:       "
                    << lastQueryTime << "\n"
                    << "\n";
    } else {
        report::StatRecord stat(record.domain(),
                                0,
                                record.isSuccess() ? 1 : 0,
                                record.isSuccess() ? 0 : 1,
                                record.isSuccess() ? record.duration() : 0,
                                now,
                                now);

        LOG_S(INFO) << "\n=== Domain " << record.domain() << "===\n"
                    << "  Average query time (ms):         "
                    << stat.total_duration << "\n"
                    << "  Std Deviation query time (ms):   0\n"
                    << "  Total # of queries:              "
                    << stat.number_success + stat.number_fail << "\n"
                    << "  Timestamp of first / last query: now\n"
                    << "\n";

        ret = upsertStatsRecordByDomain(stat, true);
    }

    // Write this query to journaling table if enblaed, failure at this
    // step does not affect return value of this function
    if (d_journalQuery) {
        QueryRecord qr(record.domain(),
                       record.isSuccess() ? "success" : "fail",
                       record.duration(),
                       now);
        writeQueryRecord(&qr);
    }

    return ret;
}

}
}
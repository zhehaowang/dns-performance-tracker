#ifndef INCLUDED_REPORT_DBRECORD
#define INCLUDED_REPORT_DBRECORD

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>

namespace dpt {
namespace report {

sql_create_7(domain_stats, 1, 7,
    mysqlpp::sql_varchar, domain,
    mysqlpp::sql_double, variance,
    mysqlpp::sql_bigint, number_success,
    mysqlpp::sql_bigint, number_fail,
    mysqlpp::sql_bigint, total_duration,
    mysqlpp::sql_timestamp, first_time,
    mysqlpp::sql_timestamp, last_time);

typedef domain_stats DBRecord;

}
}

#endif
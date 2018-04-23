#include <query_sender.h>
#include <query_util.h>
#include <report_record.h>

#include <ldns/ldns.h>
#include <iostream>

#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

namespace dpt {
namespace query {

namespace {
const int RANDOM_CHAR_LEN = 8;
}

int QuerySender::sendQuery(const std::string& dest) {
    std::string queryDomain = QueryUtil::prependRandomDomain(dest, RANDOM_CHAR_LEN);

    ldns_rdf *domain = ldns_dname_new_frm_str(queryDomain.c_str());
    if (!domain) {
        LOG_S(WARNING) << dest << " cannot be parsed" << std::endl;
        return -1;
    }

    ldns_resolver *resolver = NULL;
    if (ldns_resolver_new_frm_file(&resolver, NULL) != LDNS_STATUS_OK) {
        LOG_S(WARNING) << "Resolver cannot be created" << std::endl;
        return -1;
    }

    std::chrono::milliseconds now =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());

    ldns_pkt *response = ldns_resolver_query(resolver,
                                             domain,
                                             LDNS_RR_TYPE_A,
                                             LDNS_RR_CLASS_IN,
                                             LDNS_RD);

    std::chrono::milliseconds elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()) - now;

    if (!response) {
        LOG_S(WARNING) << "Query " << queryDomain << " failed";

        report::Record record(dest, 0, false);
        if (p_reporter) {
            p_reporter->reportRecord(record);
        }
    } else {
        LOG_S(DEBUG) << "Query " << queryDomain << " succeeded";

        report::Record record(dest, elapsed.count(), true);
        if (p_reporter) {
            p_reporter->reportRecord(record);
        }
        ldns_pkt_free(response);
    }

    ldns_rdf_deep_free(domain);
    return 0;
}

}
}
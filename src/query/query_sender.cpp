#include <query_sender.h>

#include <report_record.h>

#include <ldns/ldns.h>

#include <iostream>

namespace dpt {
namespace query {

// https://git.nlnetlabs.nl/ldns/tree/examples/ldns-mx.c
int QuerySender::sendQuery(const std::string& dest) {
    ldns_rdf *domain = ldns_dname_new_frm_str(dest.c_str());
    if (!domain) {
        std::cerr << dest << " cannot be parsed" << std::endl;
        return -1;
    }

    ldns_resolver *resolver = NULL;
    if (ldns_resolver_new_frm_file(&resolver, NULL) != LDNS_STATUS_OK) {
        return -1;
    }

    std::chrono::milliseconds now =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());

    ldns_pkt *response = ldns_resolver_query(resolver,
                                             domain,
                                             LDNS_RR_TYPE_A,   // a host address
                                             LDNS_RR_CLASS_IN, // Internet
                                             LDNS_RD);         // recursion desired
    // http://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml
    std::chrono::milliseconds elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()) - now;

    if (!response) {
        std::cerr << "Query for " << domain << " failed" << std::endl;
    } else {
        std::cerr << "Query for " << domain << " succeeded" << std::endl;
        ldns_pkt_free(response);
    }

    report::Record record(elapsed, now);
    if (p_reporter) {
        p_reporter->reportRecord(record);
    }

    ldns_rdf_deep_free(domain);
    return 0;
}

}
}
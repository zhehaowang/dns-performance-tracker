#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include <query_sender.h>
//#include <report_cmdlinereporter.h>
#include <report_mysqlreporter.h>

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace dpt;

    std::cout << "main\n";

   	// query::QuerySender query(std::make_shared<report::CmdlineReporter>());
   	std::shared_ptr<report::MySQLReporter> reporter =
   		std::make_shared<report::MySQLReporter>("dpt", "localhost", "root", "12345");
   	
   	/*
   	vector<report::DBRecord> res;
   	reporter->readStatsRecordByDomain(&res, "domain");
   	std::cout << res.size() << "\n";

   	report::DBRecord dbr("domain", 1000, 1, 0, 10, mysqlpp::sql_timestamp("1998-09-25"), mysqlpp::sql_timestamp("1998-09-25"));
   	reporter->upsertStatsRecordByDomain(dbr, false);
	*/

   	query::QuerySender query(reporter);
    query.sendQuery("google.com");

    return 0;
}
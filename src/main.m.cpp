#include <iostream>
#include <string>
#include <memory>

#include <query_sender.h>
//#include <report_cmdlinereporter.h>
#include <report_mysqlreporter.h>

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace dpt;

    std::cout << "main\n";

   	// query::QuerySender query(std::make_shared<report::CmdlineReporter>());
   	query::QuerySender query(std::make_shared<report::MySQLReporter>("dpt", "localhost", "root", ""));
    query.sendQuery("123.www.google.com");

    return 0;
}
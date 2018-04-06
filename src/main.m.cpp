#include <iostream>
#include <string>

#include <query_sender.h>
#include <report_cmdlinereporter.h>

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace dpt;

    std::cout << "main\n";

    report::CmdlineReporter reporter;

    query::QuerySender query(&reporter);
    query.sendQuery("123.www.google.com");

    return 0;
}
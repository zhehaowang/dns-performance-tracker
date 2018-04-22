#include <report_cmdlinereporter.h>

#include <iostream>
#include <string>

namespace dpt {
namespace report {

int CmdlineReporter::reportRecord(const Record& record)
{
	std::cout << "time elapsed in this query: " << record.duration() << "ms\n";
	return 0;
}

}
}
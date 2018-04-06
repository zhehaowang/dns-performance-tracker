#include <report_cmdlinereporter.h>

#include <iostream>
#include <string>

namespace dpt {
namespace report {

int CmdlineReporter::reportRecord(const Record& record)
{
	std::cout << "time elapsed in this query: " << record.durationAsUInt64() << "ms\n";
	std::cout << "time of this query: " << record.timestampAsUInt64() << "ms\n";
	return 0;
}

}
}
#ifndef INCLUDED_REPORT_CMDLINEREPORTER
#define INCLUDED_REPORT_CMDLINEREPORTER

#include <report_reporterinterface.h>

namespace dpt {
namespace report {

class CmdlineReporter : public ReporterInterface {
  public:
  	CmdlineReporter();

  	virtual int reportRecord(const Record& record);
  private:

};

inline
CmdlineReporter::CmdlineReporter()
{}

}
}

#endif
#ifndef INCLUDED_REPORT_REPORTERINTERFACE
#define INCLUDED_REPORT_REPORTERINTERFACE

#include <report_record.h>

namespace dpt {
namespace report {

class ReporterInterface {
  public:
  	virtual ~ReporterInterface();

  	virtual int reportRecord(const Record& record) = 0;
};

inline
ReporterInterface::~ReporterInterface()
{}

}
}

#endif
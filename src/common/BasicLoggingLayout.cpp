#include "firtex/common/BasicLoggingLayout.h"
#include "firtex/utility/DateTime.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(common);

char BasicLoggingLayout::LEVEL_PRINT_PREFIX[][20] = 
{
    " [UNKNOW] ",
    " [FATAL] ",
    " [CRITICAL] ",
    " [ERROR] ",
    " [WARN] ",
    " [NOTICE] ",
    " [INFO] ",
    " [DEBUG] ",
    " [TRACE] ",
};

void BasicLoggingLayout::format(std::ostringstream& message, const LoggingEvent& event)
{
    std::string sTime;
    sTime.reserve(20);
    DateTime dt(event.timestamp);
    dt.format(sTime, DateTime::SORTABLE_FORMAT);

    message << sTime << LEVEL_PRINT_PREFIX[event.level]
            << "[" << event.loggerName << "]" 
            << "[" << event.file << ", " << event.line << "]: "
            << event.message << std::endl;
}

FX_NS_END


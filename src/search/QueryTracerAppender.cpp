#include "firtex/search/QueryTracerAppender.h"
#include "firtex/common/LoggingLayout.h"
#include "firtex/common/PatternLoggingLayout.h"

FX_NS_USE(common);
FX_NS_DEF(search);

QueryTracerAppender::QueryTracerAppender() 
{
    PatternLoggingLayout* pLayout = new PatternLoggingLayout();
    pLayout->setPattern("%d [%l] %m");
    setLayout(pLayout);
}

QueryTracerAppender::~QueryTracerAppender() 
{
}

void QueryTracerAppender::append(const LoggingEvent& event)
{
    LoggingLayout& layout = getLayout();
    std::ostringstream oss;
    layout.format(oss, event);
    m_traceInfo.push_back(oss.str());
}

FX_NS_END


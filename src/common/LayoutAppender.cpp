#include "firtex/common/LayoutAppender.h"
#include "firtex/common/BasicLoggingLayout.h"
#include "firtex/common/PatternLoggingLayout.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(utility);

FX_NS_DEF(common);

LayoutAppender::LayoutAppender() 
    : m_pLayout(NULL)
{
    setLayout("layout.basic");
}

LayoutAppender::LayoutAppender(const std::string& sLayout) 
    : m_pLayout(NULL)
{
    setLayout(sLayout);
}

LayoutAppender::~LayoutAppender() 
{
    if (m_pLayout)
    {
        delete m_pLayout;
        m_pLayout = NULL;
    }
}

void LayoutAppender::setLayout(const std::string& sLayout)
{
    LoggingLayout* pLayout = NULL;
    if (sLayout.empty() || (!strCompareNoCase(sLayout.c_str(), "basic")))
    {
        pLayout = new BasicLoggingLayout();
    }
    else
    {
        pLayout = new PatternLoggingLayout();
    }
    
    setLayout(pLayout);
}

void LayoutAppender::setLayout(LoggingLayout* pLayout)
{
    if (m_pLayout)
    {
        delete m_pLayout;
    }
    m_pLayout = pLayout;
}

FX_NS_END


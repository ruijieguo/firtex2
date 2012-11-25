#include "firtex/common/LoggingLayoutFactory.h"
#include "firtex/common/BasicLoggingLayout.h"
#include "firtex/common/PatternLoggingLayout.h"

using namespace std;
FX_NS_DEF(common);

LoggingLayoutFactory::LoggingLayoutFactory() 
{
    registerCreator(BasicLoggingLayout::Creator::instance());
    registerCreator(PatternLoggingLayout::Creator::instance());
}

LoggingLayoutFactory::~LoggingLayoutFactory() 
{
}

void LoggingLayoutFactory::registerCreator(LoggingLayout::Creator* pCreator)
{
    m_creators.insert(make_pair(pCreator->getIdentifier(), pCreator));
}

LoggingLayout* LoggingLayoutFactory::createLoggingLayout(const string& sIdent)
{
    CreatorMap::const_iterator it = m_creators.find(sIdent);
    if (it != m_creators.end())
    {
        return it->second->create();
    }
    return NULL;
}

FX_NS_END


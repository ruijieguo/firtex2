#include "firtex/common/AppenderFactory.h"
#include "firtex/common/ConsoleAppender.h"
#include "firtex/common/FileAppender.h"

using namespace std;
FX_NS_DEF(common);

AppenderFactory::AppenderFactory() 
{
    registerCreator(ConsoleAppender::Creator::instance());
    registerCreator(FileAppender::Creator::instance());
}

AppenderFactory::~AppenderFactory() 
{
}

void AppenderFactory::registerCreator(Appender::Creator* pCreator)
{
    m_creators.insert(make_pair(pCreator->getIdentifier(), pCreator));
}

Appender* AppenderFactory::createAppender(const tstring& sIdent)
{
    CreatorMap::const_iterator it = m_creators.find(sIdent);
    if (it != m_creators.end())
    {
        return it->second->create();
    }
    return NULL;
}

FX_NS_END


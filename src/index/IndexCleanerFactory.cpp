#include "firtex/index/IndexCleanerFactory.h"
#include "firtex/index/KeepByCommitIndexCleaner.h"
#include "firtex/utility/StringUtils.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(store, IndexCleanerFactory);

IndexCleanerFactory::IndexCleanerFactory() 
{
    CreatorPtr pTmp(new KeepByCommitIndexCleaner::Creator);
    registerIndexCleaner(pTmp);
}

IndexCleanerFactory::~IndexCleanerFactory() 
{
}

IndexCleaner* IndexCleanerFactory::createIndexCleaner(const std::string& sIdent)
{
    string str = sIdent;
    if (!startsWithNoCase(sIdent, string("cleaner.")))
    {
        str = "cleaner." + str;
    }
    CreatorMap::const_iterator it = m_fileSysMap.find(str);
    if (it != m_fileSysMap.end())
    {
        CreatorPtr pCreator = it->second;
        return pCreator->create();
    }
    return NULL;
}

void IndexCleanerFactory::registerIndexCleaner(const CreatorPtr& pCreator)
{
    string sIdent = pCreator->getIdentifier();
    CreatorMap::iterator it = m_fileSysMap.find(sIdent);
    if (it != m_fileSysMap.end())
    {
        m_fileSysMap.erase(it);
        FX_LOG(WARN, "File system : [%s] already exists.", sIdent.c_str());
    }
    m_fileSysMap.insert(make_pair(sIdent, pCreator));
}

FX_NS_END


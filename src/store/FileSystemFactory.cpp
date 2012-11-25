#include "firtex/store/FileSystemFactory.h"
#include "firtex/store/BlockFileSystem.h"
#include "firtex/store/MMapFileSystem.h"

using namespace std;
FX_NS_DEF(store);

SETUP_STREAM_LOGGER(store, FileSystemFactory);

FileSystemFactory::FileSystemFactory() 
{
    registerFileSystem(new BlockFileSystem::Creator);
    registerFileSystem(new MMapFileSystem::Creator);
}

FileSystemFactory::~FileSystemFactory() 
{
}

FileSystem* FileSystemFactory::createFileSystem(const std::string& sIdent)
{
    CreatorMap::const_iterator it = m_fileSysMap.find(sIdent);
    if (it != m_fileSysMap.end())
    {
        CreatorPtr pCreator = it->second;
        return pCreator->create();
    }
    return NULL;
}

void FileSystemFactory::registerFileSystem(const CreatorPtr& pCreator)
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


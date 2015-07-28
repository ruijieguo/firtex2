#include "firtex/store/MMapFileSystem.h"
#include "firtex/store/MMapFileInputStream.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(thread);

FX_NS_DEF(store);

SETUP_STREAM_LOGGER(store, MMapFileSystem);

MMapFileSystem::MMapFileSystem() 
{
}

MMapFileSystem::~MMapFileSystem() 
{
}

InputStreamPtr MMapFileSystem::openFile(const std::string& sFileName)
{
    FastMutex::Guard lock(m_mmapLock);        

    FilePtr pFile;
    MMapFilePtr pMMapFile;
    string str = toLower(sFileName);
    MMapFileMap::iterator it = m_mmapfiles.find(str);
    if (it != m_mmapfiles.end())
    {
        pFile = it->second.first;
        pMMapFile = it->second.second;
    }

    if (!pFile)
    {
        pFile = doOpenFile(sFileName);
        if (!pFile)
        {
            return InputStreamPtr();
        }
        if (pFile->fileActualSize == 0)
        {
            BlockFileSystem::closeFile(pFile);
            return InputStreamPtr();
        }

        pMMapFile.reset(new MMapFile());
        pMMapFile->open(pFile->fileHandle, MMapFile::AM_READ,
                        (size_t)pFile->fileActualSize);
        
        m_mmapfiles.insert(make_pair(pFile->fileName, 
                        make_pair(pFile, pMMapFile)));
    }
    
    MMapFileInputStreamPtr pMMpInStream(new MMapFileInputStream(
                    *this, pFile, pMMapFile));
    
    return std::dynamic_pointer_cast<InputStream>(pMMpInStream);
}

void MMapFileSystem::closeFile(FilePtr& pFile)
{
    FastMutex::Guard lock(m_mmapLock);
    MMapFileMap::iterator it = m_mmapfiles.find(pFile->fileName);
    if (it != m_mmapfiles.end())
    {
        if (it->second.second.use_count() == 1)
        {
            m_mmapfiles.erase(it);
            BlockFileSystem::closeFile(pFile);
        }
    }
    else
    {
        BlockFileSystem::closeFile(pFile);
    }
}

FX_NS_END


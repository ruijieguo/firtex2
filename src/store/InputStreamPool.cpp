#include "firtex/store/InputStreamPool.h"

FX_NS_USE(thread);
FX_NS_USE(utility);

FX_NS_DEF(store);

SETUP_STREAM_LOGGER(store, InputStreamPool);

InputStreamPool::InputStreamPool(const FileSystemPtr& pFileSys, 
                                 size_t nPoolSize)
    : m_pFileSystem(pFileSys)
    , m_nPoolSize(nPoolSize)
    , m_nOpenedStreamCount(0)
{
}

InputStreamPool::~InputStreamPool() 
{
    for (StreamMap::iterator it = m_streamMap.begin();
         it != m_streamMap.end(); ++it)
    {
        StreamVector& streams = it->second;
        for (size_t i = 0; i < streams.size(); ++i)
        {
            try
            {
                streams[i]->close();
            }
            catch(const FileIOException& e)
            {
                FX_LOG(ERROR, "Close file FAILED: [%s]", e.what().c_str());
            }
        }
        streams.clear();
    }
    m_streamMap.clear();

    m_nOpenedStreamCount = 0;
}

InputStreamPtr InputStreamPool::getInputStream(const std::string& sFileName) const
{
    {
        ScopedLock<FastMutex> lock(m_mutex);

        InputStreamPtr pInStream = doGetInputStream(sFileName);
        if (pInStream)
        {
            return pInStream;
        }
    }
    return m_pFileSystem->openFile(sFileName);
}

void InputStreamPool::releaseInputStream(InputStreamPtr& pInputStream) const
{
    {
        ScopedLock<FastMutex> lock(m_mutex);

        --m_nOpenedStreamCount;
        StreamVector& streams = m_streamMap[pInputStream->getFilePath()];
        if (streams.size() < m_nPoolSize)
        {
            InputStreamPtr pTmpPtr = pInputStream;
            streams.push_back(pTmpPtr);
            return;
        }
    }
    pInputStream.reset();
}

InputStreamPool::InputStreamPair 
InputStreamPool::getInputStreams(const StringPair& fileNames) const
{
    InputStreamPtr pInStream1;
    InputStreamPtr pInStream2;
    {
        ScopedLock<FastMutex> lock(m_mutex);

        pInStream1 = doGetInputStream(fileNames.first);
        pInStream2 = doGetInputStream(fileNames.second);
        if (pInStream1 && pInStream2)
        {
            return std::make_pair(pInStream1, pInStream2);
        }
    }
    
    if (!pInStream1)
    {
        pInStream1 = m_pFileSystem->openFile(fileNames.first);
    }

    if (!pInStream2)
    {
        pInStream2 = m_pFileSystem->openFile(fileNames.second);
    }

    return std::make_pair(pInStream1, pInStream2);
}

FX_NS_END


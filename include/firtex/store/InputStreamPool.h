//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-05-31 21:00:31

#ifndef __FX_INPUTSTREAMPOOL_H
#define __FX_INPUTSTREAMPOOL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/FileSystem.h"
#include "firtex/thread/FastMutex.h"
#include <map>

FX_NS_DEF(store);

class InputStreamPool
{
public:
    const static size_t DEFAULT_POOL_SIZE = 16;
    
    typedef std::pair<InputStreamPtr, InputStreamPtr> InputStreamPair;
    typedef std::pair<std::string, std::string> StringPair;

public:
    /**
     * Constructor
     *
     * @param pFileSys file system instance
     * @param nPoolSize the pool size
     */
    InputStreamPool(const FileSystemPtr& pFileSys, 
                    size_t nPoolSize = DEFAULT_POOL_SIZE);
    ~InputStreamPool();

public:
    /**
     * Get input stream from pool or open a new stream.
     * This function is thread-safe.
     *
     * @param sFileName the file name of input stream
     */
    InputStreamPtr getInputStream(const std::string& sFileName) const;

    /**
     * Get input stream pair from pool or open new streams.
     * This function is thread-safe.
     *
     * @param fileNames the file names of input stream
     */
    InputStreamPair getInputStreams(const StringPair& fileNames) const;


    /**
     * Return the input stream to pool or close the stream
     * This function is thread-safe.
     *
     * @param pInputStream input stream to release
     */
    void releaseInputStream(InputStreamPtr& pInputStream) const;

private:
    InputStreamPtr doGetInputStream(const std::string& sFileName) const ;

private:
    typedef std::vector<InputStreamPtr> StreamVector;
    typedef std::map<std::string, StreamVector> StreamMap;

    /// File system instance
    mutable FileSystemPtr m_pFileSystem;

    /// For thread-safe
    mutable FX_NS(thread)::FastMutex m_mutex;

    /// Pool the opened input streams
    mutable StreamMap m_streamMap;

    /// Max pool size for each file
    size_t m_nPoolSize;
    
    /// Number of opened input streams
    mutable size_t m_nOpenedStreamCount;
    
private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(InputStreamPool);

/////////////////////////////////////////
//
inline InputStreamPtr InputStreamPool::doGetInputStream(const std::string& sFileName) const
{
    ++m_nOpenedStreamCount;
    StreamMap::iterator it = m_streamMap.find(sFileName);
    if (it != m_streamMap.end())
    {
        StreamVector& streams = it->second;
        if (streams.size() > 0)
        {
            InputStreamPtr pInStream = streams.back();
            streams.pop_back();
            return pInStream;
        }
        else
        {
            streams.reserve(m_nPoolSize);
        }
    }
    
    return InputStreamPtr();
}

FX_NS_END

#endif //__FX_INPUTSTREAMPOOL_H

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
// Created : 2011-06-19 18:37:43

#ifndef __FX_MONITORDIRECTORYFILEFETCHER_H
#define __FX_MONITORDIRECTORYFILEFETCHER_H

#include <set>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/Runnable.h"
#include "firtex/thread/Thread.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include "firtex/extension/collection/FileFetcher.h"
#include "firtex/extension/collection/FileFilter.h"

FX_NS_DEF(collection);

class MonitorDirectoryFileFetcher : public FileFetcher, public FX_NS(thread)::Runnable
{
public:
    const static size_t DEFAULT_FILE_QUEUE_SIZE = 1000;
    const static size_t DEFAULT_MONITOR_TIME_INTERVAL = 10 * 1000; //10 seconds
    const static size_t MAX_PATH_LENGTH = 512;

    static std::string INCREMENTAL_LIST;

public:
    MonitorDirectoryFileFetcher();
    ~MonitorDirectoryFileFetcher();

public:
    /**
     * Initialize the fetcher
     * 
     * @param sDirectory directory path
     * @param pFileFilter used to filter out the unneeded files
     */
    void init(const std::string& sDirectory, const FileFilterPtr& pFileFilter);

    /**
     * Retrieval one raw document and move to next
     *
     * @param pRawDoc store extracted raw document
     *
     * @return true if there is one document, otherwise return false
     */
    virtual bool fetchNext(FX_NS(processor)::RawDocumentPtr& pRawDoc);

    /// Stop monitor
    virtual void stop();

private:
    void run();
//    void updateDirectory();
//    void checkUpdate(const std::string& sDirPath);
    void checkIncrementList();
    void buildIncrementList();

private:
    typedef FX_NS(utility)::BoundedConcurrentQueue<std::string> FileQueue;
    typedef std::set<std::string> DirectorySet;

    /// Root directory to monitor
    std::string m_sRootDir;
    
    /// File path of increment list
    std::string m_sIncList;

    /// Last offset in increment list
    int32_t m_nLastOffset;

    /// Use to filer files
    FileFilterPtr m_pFileFilter;

    /// Thread for monitoring file modification
    FX_NS(thread)::Thread m_monitorThread;

    /// All processing files 
    FileQueue m_fileQueue;

    /// Store all monitoring directories
    DirectorySet m_directories;

    /// Store current file path
    std::string m_sCurFile;
    
    /// Use to stop while loop
    bool m_bStop;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(MonitorDirectoryFileFetcher);

FX_NS_END

#endif //__FX_MONITORDIRECTORYFILEFETCHER_H

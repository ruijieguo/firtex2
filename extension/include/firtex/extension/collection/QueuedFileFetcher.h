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
// Created : 2015-07-04 22:16:43

#ifndef __FX_QUEUEDFILEFETCHER_H
#define __FX_QUEUEDFILEFETCHER_H

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

class QueuedFileFetcher : public FileFetcher, public FX_NS(thread)::Runnable
{
public:
    const static size_t DEFAULT_FILE_QUEUE_SIZE = 100;

public:
    QueuedFileFetcher();
    ~QueuedFileFetcher();

public:
    /**
     * Retrieval one raw document and move to next
     *
     * @param pRawDoc store extracted raw document
     *
     * @return true if there is one document, otherwise return false
     */
    virtual bool fetchNext(FX_NS(processor)::RawDocumentPtr& pRawDoc);

    /// Stop 
    virtual void stop();

private:
    typedef FX_NS(utility)::BoundedConcurrentQueue<std::string> FileQueue;

    /// All processing files 
    FileQueue m_fileQueue;

    /// Use to stop while loop
    bool m_bStop;
private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(QueuedFileFetcher);

FX_NS_END

#endif //__FX_QUEUEDFILEFETCHER_H

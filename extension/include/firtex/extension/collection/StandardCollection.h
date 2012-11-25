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
// Created : 2011-06-18 21:40:49

#ifndef __FX_STANDARDCOLLECTION_H
#define __FX_STANDARDCOLLECTION_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/Runnable.h"
#include "firtex/extension/collection/Collection.h"
#include "firtex/extension/collection/FileFilter.h"

FX_NS_DEF(collection);

class StandardCollection : public Collection
{
public:
    DECLARE_COLLECTION_CREATOR(StandardCollection, "standard");

public:
    /** 
     * Constructor
     *
     * @param bMonitor monitor the modification of directory or not 
     */
    StandardCollection();
    ~StandardCollection();

public:
    /**
     * Create file fetcher 
     * Implement by derived class.
     * 
     * @return document processor object, caller the ownership.
     */
    virtual FileFetcher* createFileFetcher();

    /**
     * Create document processor for processing raw document. 
     * 
     * @return standard document processor object, caller the ownership.
     */
    virtual FX_NS(processor)::DocumentProcessor* createDocumentProcessor();

    /**
     * Create file filter for filtering unwanted documents
     * 
     * @param sParam parameter for file filter
     *
     * @return file filter object, caller the ownership.
     */
    virtual FileFilter* createFileFilter(const std::string& sParam);

    /// Initialize resource
    virtual void doInit(const CollectionConfigurePtr& pConfigure); 

    /**
     * Initialize parameter for collection
     * 
     * @param sParam parameter string for collection.
     *        format: key1=value11, value12,...; key2=value21,value22,...;...
     */
    virtual void doInitParam(const std::string& sParam);

    /// Wait background processor threads to stop if in non-blocking mode
    virtual void waitStop();

private:
    FileFetcherPtr m_pFileFetcher;

    bool m_bMonitor;
private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_STANDARDCOLLECTION_H

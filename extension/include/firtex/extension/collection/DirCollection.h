//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : Ruijie Guo
// Email        : ruijieguo@gmail.com
// Created      : 2006/7/3
//
#ifndef __FX_DIRCOLLECTION_H
#define __FX_DIRCOLLECTION_H

#include "firtex/extension/collection/Collection.h"
#include "firtex/common/Logger.h"
#include "firtex/extension/processor/DocumentProcessor.h"
#include <vector>
#include <string>

FX_NS_DEF(collection);

class DirCollection : public Collection
{
public:
    DECLARE_COLLECTION_CREATOR(DirCollection, "directory");

public:
    /**
     * Constructor
     */
    DirCollection();
    virtual ~DirCollection();

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
     * Implement by derived class.
     * 
     * @return document processor object, caller the ownership.
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

private:
    void initProcessors(const std::string& sDocType);

private:
    FX_NS(processor)::DocumentProcessorPtr m_pDocProcessor;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DIRCOLLECTION_H

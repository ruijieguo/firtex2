/**
 * Copyright (C) 2007 FirteX Development Team. All rights reserved.
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 *
 */

#ifndef __FXCOLLECTION_H
#define __FXCOLLECTION_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/document/DocumentType.h"
#include "firtex/index/Index.h"
#include "firtex/thread/Thread.h"
#include "firtex/thread/ThreadPool.h"
#include "firtex/config/Configurable.h"
#include "firtex/extension/processor/DocumentProcessor.h"
#include "firtex/extension/collection/FileFetcher.h"
#include "firtex/extension/collection/CollectionConfigure.h"
#include "firtex/extension/collection/ProgressObserver.h"
#include "firtex/extension/collection/FileFilter.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/BoundedConcurrentQueue.h"
#include <map>

FX_NS_DEF(collection);

class CollectionTemplate;

#define DECLARE_COLLECTION_CREATOR(cname, id)                           \
    class Creator : public Collection::Creator,                         \
                    public FX_NS(utility)::Singleton<cname::Creator>    \
    {                                                                   \
    public:                                                             \
        Creator()                                                       \
        {                                                               \
            m_identifier += _T(".");                                    \
            m_identifier += id;                                         \
        }                                                               \
    public:                                                             \
        Collection* create() const                                      \
        {return new cname();}                                           \
        DECLARE_LAZY_SINGLETON(cname::Creator);                         \
    }

class Collection;
DEFINE_TYPED_PTR(Collection);

/**
 * @class Collection
 * @author Ruijie Guo
 * @email ruijieguo@gmail.com
 * @date <2005-11-09>
 */
class Collection : public FX_NS(thread)::Runnable
{

public:
    class Creator
    {
    protected:
        Creator() : m_identifier(_T("collection")) {}

    public:
        virtual ~Creator(){}

    public:
        /**
         * create a collection object
         * @param path path of collection
         */
        virtual Collection* create() const = 0;

        /** 
         * get the identifier of collection
         */
        virtual const tstring getIdentifier() const {return m_identifier;}

    protected:
        tstring m_identifier;
    };

public:
    Collection();
    virtual ~Collection();

public:
    /**
     * Initialize collection
     *
     * @param pConfigure configuration of the collection
     */
    virtual void init(const CollectionConfigurePtr& pConfigure);    

public:
    /**
     * Create file fetcher 
     * Implement by derived class.
     * 
     * @return document processor object, caller the ownership.
     */
    virtual FileFetcher* createFileFetcher() = 0;

    /**
     * Create document processor for processing raw document. 
     * Implement by derived class.
     * 
     * @return document processor object, caller the ownership.
     */
    virtual FX_NS(processor)::DocumentProcessor* createDocumentProcessor() = 0;

    /**
     * Create file filter for filtering unwanted documents
     * Implement by derived class.
     * 
     * @param sParam parameter for file filter
     *
     * @return file filter object, caller the ownership.
     */
    virtual FileFilter* createFileFilter(const std::string& sParam) = 0;
    
    /// Give an initialize chance to derived class
    virtual void doInit(const CollectionConfigurePtr& pConfigure) = 0; 

    /**
     * Initialize parameter for collection
     * 
     * @param sParam parameter string for collection.
     *        format: key1=value11, value12,...; key2=value21,value22,...;...
     */
    virtual void doInitParam(const std::string& sParam) = 0;

public:
    /// Return configuration
    inline const CollectionConfigurePtr& getConfigure() const;

    /// Return file fetcher object
    inline FileFetcherPtr& getFileFetcher();

    /// Return file filter object
    inline FileFilterPtr& getFileFilter();

public:
    /**
     * Scan and process documents in collection
     *
     * @param bBlock if true, blocked until all documents processed, 
     *               if false, return immediately.
     */
    virtual void scan(bool bBlock);

    /// Stop background processor threads if in non-blocking mode
    virtual void stop();

    /// Wait background processor threads to stop if in non-blocking mode
    virtual void waitStop();

public:
    /**
     * Set a progress observer
     *
     * @param pObserver progress observer of the collection
     */
    virtual void setObserver(const ProgressObserverPtr& pObserver);

protected:
    /// Thread entry
    virtual void run();

    /// Scan collection
    void doScan();

    /// Process documents
    void doProcess();

    /// Initialize process threads
    void initProcessThreads();

    /// Validate the configuration
    void sanityCheck(const CollectionConfigurePtr& pConfigure);
    
    /**
     * set parameter for collection
     * @param pszParam parameter for collection. 
     *        format:param_name=param_value
     */
    virtual void initParam(const std::string& sParam);

public:
    /**
     * Build index by configure file
     *
     * @param sConfFile configure file name
     * @param pProgressObserver progress observer
     * @param bBlock block until the build is completed
     */
    static CollectionPtr buildIndex(const std::string& sConfFile,
                                    const ProgressObserverPtr& pProgressObserver = ProgressObserverPtr(),
                                    bool bBlock = true);

    /**
     * build index by configure
     *
     * @param pConfigure configure
     * @param pProgressObserver progress observer
     * @param bBlock block until the build is completed
     */
    static CollectionPtr buildIndex(const CollectionConfigurePtr& pConfigure,
                           const ProgressObserverPtr& pProgressObserver = ProgressObserverPtr(),
                           bool bBlock = true);

    /**
     * Build a collection object by configure file
     * @param sConfigFile configure file name
     * @return Collection object
     */
    static CollectionPtr buildCollection(const std::string& sConfigFile);

protected:
    static void makePathAbsolute(const std::string& sBasePath, std::string& sPath);

private:
    typedef FX_NS(utility)::BoundedConcurrentQueue<FX_NS(processor)::RawDocumentPtr> InputDocQueue;
    DEFINE_TYPED_PTR(InputDocQueue);

    class ProcessorRunner : public FX_NS(thread)::Runnable
    {
    public:
        ProcessorRunner(Collection& col)
            : m_collection(col)
        {
        }

    public:
        void run()
        {
            m_collection.doProcess();
        }

    private:
        Collection& m_collection;
    };
    DEFINE_TYPED_PTR(ProcessorRunner);

    typedef std::vector<ProcessorRunnerPtr> ProcessorVector;

private:
    FX_NS(index)::IndexPtr m_pIndex;
    FX_NS(index)::IndexWriterPtr m_pWriter;

    CollectionConfigurePtr m_pConfigure;
    FileFetcherPtr m_pFileFetcher;
    FileFilterPtr m_pFileFilter;
    ProcessorVector m_processors;
    InputDocQueuePtr m_pInputDocQueue;
    
    FX_NS(thread)::ThreadPoolPtr m_pThreadPool;
    FX_NS(thread)::ThreadPtr m_pScanThread;
    
    ProgressObserverPtr m_pObserver;

    bool m_bStop;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////////////////
///inline function

inline const CollectionConfigurePtr& Collection::getConfigure() const
{
    return m_pConfigure;
}

inline FileFetcherPtr& Collection::getFileFetcher()
{
    return m_pFileFetcher;
}

inline FileFilterPtr& Collection::getFileFilter()
{
    return m_pFileFilter;
}

FX_NS_END

#endif

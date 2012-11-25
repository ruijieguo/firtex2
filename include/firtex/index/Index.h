//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail
// Created	: 2005/12/07
//
//

#ifndef __FXINDEX_H
#define __FXINDEX_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/document/Document.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/IndexWriter.h"
#include "firtex/index/IndexReader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/SynchronizedObject.h"
#include "firtex/index/DeletedDocumentFilter.h"
#include "firtex/thread/RWLock.h"


FX_NS_DEF(analyzer);
class AnalyzerMapper;
FX_NS_END

FX_NS_DEF(index);

class BarrelsInfo;

class Index
{
public:
    typedef enum
    {
        READ   = 0x10, //read index only
        WRITE  = 0x20, //build index
        APPEND = 0x40, //incremental build index
        RDWR   = READ | WRITE, //read/write index in real-time mode
    }AccessMode;

public:
    /**
     * Remove index 
     * @param sIndexPath index path
     */
    static void remove(const std::string& sIndexPath);

    /**
     * Remove index 
     * @param pFileSys file system instance
     */
    static void remove(FX_NS(store)::FileSystemPtr& pFileSys);

    /**
     * Determine an index exists or not
     * @return true if exist
     */
    static bool exists(const std::string& sIndexPath);

public:
    Index(void);
    virtual ~Index(void);

public:
    /**
     * Open index in specified mode
     * @param sIndexPath index location path
     * @param am access mode
     * @param pDocSchema document schema, optional when open in READ mode
     * @throw throw FirteXException if failed.
     */
    void open(const std::string& sIndexPath, AccessMode am,
              const FX_NS(document)::DocumentSchema* pDocSchema);

    /**
     * Open index in specified mode
     * @param pFileSys file system where index stored
     * @param am access mode
     * @param pDocSchema document schema, optional when open in READ mode
     * @throw throw FirteXException if failed.
     */
    void open(FX_NS(store)::FileSystemPtr& pFileSys, AccessMode am,
              const FX_NS(document)::DocumentSchema* pDocSchema);

    /**
     * Close index
     */
    virtual void close();

public:
    /// Return const reference to index barrel keeper
    inline const IndexBarrelKeeperPtr& getIndexBarrelKeeper() const;

    /// Return reference to index barrel keeper
    inline IndexBarrelKeeperPtr& getIndexBarrelKeeper();

    /**
     * Return document schema of index 
     */
    const FX_NS(document)::DocumentSchema* getDocSchema() const;

    /**
     * Return file system of index
     */
    FX_NS(store)::FileSystemPtr getFileSystem() const {return m_pFileSys;}

    /** 
     * Return analyzer mapper which used to map field id to analyzer
     * @return const reference to AnalyzerMapper object
     * @see AnalyzerMapper
     */
    const FX_NS(analyzer)::AnalyzerMapper* getAnalyzerMapper() const;

    /**
     * Return component builder for building component of field
     * @see ComponentBuilder
     */
    const ComponentBuilder* getComponentBuilder() const;

    /**
     * Get deleted document filter
     */
    const DeletedDocumentFilter* getDeletedDocFilter() const;

public:

    /**
     * Acquire the index reader to read/search the index database.
     * @param bRefresh determine whether to refresh the IndexReader 
     * if the index has changed
     * @return the reader of index database when access mode is READ, 
     *         RDWR, otherwise return NULL. If the index has changed 
     *         and bRefresh is true, a new IndexReader instance will
     *         be created.
     * @see IndexReader
     */
    virtual IndexReaderPtr acquireReader(bool bRefresh = true);

    /**
     * Acquire a writer to build/modify the index database.
     * A index database can only have one writer to modify it.
     * @return a writer of index database when access mode is 
     *         WRITE, APPEND or RDWR.
     */
    virtual IndexWriterPtr acquireWriter();

protected:
    /**
     * Open reader when access mode is READ or RDWR
     */
    void openReader();

    /**
     * Open writer when access mode is WRITE, APPEND or RDWR.
     */
    void openWriter();

    /** init analyzer mapper */
    void initAnalyzerMapper();

public:
    /**
     * Read schema from file system
     */
    static FX_NS(document)::DocumentSchemaPtr readSchema(FileSystemPtr& pFileSys);

    /**
     * Read schema from file system
     */
    static void writeSchema(FX_NS(document)::DocumentSchemaPtr& pDocSchema,
                            FileSystemPtr& pFileSys);

protected:
    mutable FX_NS(thread)::RWLock m_lock;

    IndexBarrelKeeperPtr m_pIndexBarrelKeeper;

    FX_NS(store)::FileSystemPtr m_pFileSys;
    FX_NS(document)::DocumentSchemaPtr m_pDocSchema;
    IndexReaderPtr m_pReader;
    IndexWriterPtr m_pWriter;
    DeletedDocumentFilterPtr m_pDeletedDocFilter;
    AccessMode m_accessMode;

    ComponentBuilderPtr m_pComponentBuilder;

    FX_NS(analyzer)::AnalyzerMapperPtr m_pAnalyzerMapper;

    friend class IndexWriter;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(Index);

////////////////////////////////////////////////////////////
//
inline const IndexBarrelKeeperPtr& Index::getIndexBarrelKeeper() const
{
    return m_pIndexBarrelKeeper;
}

inline IndexBarrelKeeperPtr& Index::getIndexBarrelKeeper()
{
    return m_pIndexBarrelKeeper;
}

inline const DeletedDocumentFilter* Index::getDeletedDocFilter() const
{
    return m_pDeletedDocFilter.get();
}

inline const FX_NS(document)::DocumentSchema* Index::getDocSchema() const
{
    return m_pDocSchema.get();
}

inline const FX_NS(analyzer)::AnalyzerMapper* Index::getAnalyzerMapper() const
{
    return m_pAnalyzerMapper.get();
}

FX_NS_END

#endif

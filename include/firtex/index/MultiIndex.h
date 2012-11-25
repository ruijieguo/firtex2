//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn ruijieguo@gmail.com
// Created	: 2007/10/02
//

#ifndef __MULTIINDEX_H
#define __MULTIINDEX_H

#include "Index.h"
#include <vector>
#include <string>

FX_NS_DEF(index)

class MultiIndex :  public Index
{
public:
    typedef std::vector<Index*> IndexVector;

public:
    MultiIndex(void);
    virtual ~MultiIndex(void);

public:
    /** 
     * Open multi index for read
     * @param indices opened indices (READ mode)
     */
    void open(const IndexVector& indices);

    /** 
     * Open multi index for read
     * @param indexPaths index paths 
     */
    void open(const std::vector<tstring>& indexPaths);

    /**
     * get index by order
     * @return an internal index object
     */
    const IndexVector& getIndexVector() const { return m_indices;}

public:
    /**
     * \if EN
     * Acquire the reader to read/search the index database. The reader is thread-safe
     * @return the reader of index database when access mode is READ, RDWR, otherwise return NULL
     * @see IndexReader
     * \endif
     */
    virtual IndexReaderPtr acquireReader();

    /**
     * \if EN
     * Acquire a writer to build/modify the index database.
     * A index database can only have one writer to modify it.
     * @return a writer of index database when access mode is WRITE, APPEND or RDWR.
     * \endif
     */
    virtual IndexWriterPtr acquireWriter();

    BarrelsInfoPtr getBarrelsInfo() const { return m_pBarrelsInfo;}

protected:
    /**
     * add an index
     * @param pIndex index object,as an internal object
     */
    void addIndex(Index* pIndex);

    /**
     * add an index
     * @param sIndexPath index database path
     */
    void addIndex(const tstring& sIndexPath);

    /**
     * remove all indices
     * @param bDestroy destroy index object or not
     */
    void removeAllIndice(bool bDestroy);

    /**
     * add barrels
     * @param pBarrels barrels info
     */
    void addBarrels(const BarrelsInfoPtr& pBarrels);

    /** 
     * add document schema
     * @param pSchema document schema
     */
    void addSchema(const FX_NS(document)::DocumentSchema* pSchema);

protected:
    IndexVector m_indices;
    BarrelsInfoPtr m_pBarrelsInfo;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif

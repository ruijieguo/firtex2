//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-09-04 22:29:59

#ifndef __FX_DOCIDRECYCLING_H
#define __FX_DOCIDRECYCLING_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/SlicedVector.h"
#include "firtex/utility/BitVector.h"

FX_NS_DEF(index);

class DocIdRecycling
{
public:
    typedef FX_NS(utility)::SlicedVector<docid_t> Vector;

    const static size_t SLICE_SIZE = 10240;

public:
    DocIdRecycling();
    ~DocIdRecycling();

public:
    /**
     * Initialize
     */
    void init(df_t docCount, const FX_NS(utility)::BitVector* pDocFilter);
    
    /**
     * Determine whether there is any deletions
     * @return true if has deletions
     */
    bool hasDeletions() const;

    /**
     * Map old docId to new docId
     * @param docId old docId
     * @return new docId
     */
    docid_t operator[] (docid_t docId) const;
    docid_t remap(docid_t docId) const;

    /**
     * Return total doc count before recycling
     */
    df_t getDocCount() const;

    /**
     * Return total doc count after recycling
     */
    df_t getDocCountAfterRecle() const;

protected:
    void remap(docid_t oldDocId, docid_t newDocId);

protected:
    Vector* m_pDocIdMapping;

    df_t m_docCount;
    df_t m_docCountAfterRecycle;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(DocIdRecycling);

////////////////////////////////////
//
inline void DocIdRecycling::remap(docid_t oldDocId, docid_t newDocId)
{
    return m_pDocIdMapping->set((size_t)oldDocId, newDocId);
}

inline docid_t DocIdRecycling::remap(docid_t docId) const
{
    FIRTEX_ASSERT2((size_t)docId < m_pDocIdMapping->size());

    return (*m_pDocIdMapping)[(size_t)docId];
}

inline docid_t DocIdRecycling::operator[] (docid_t docId) const
{
    return remap(docId);
}

inline bool DocIdRecycling::hasDeletions() const
{
    return (m_pDocIdMapping != NULL);
}

inline df_t DocIdRecycling::getDocCount() const
{
    return m_docCount;
}

inline df_t DocIdRecycling::getDocCountAfterRecle() const
{
    return m_docCountAfterRecycle;
}


FX_NS_END

#endif //__FX_DOCIDRECYCLING_H

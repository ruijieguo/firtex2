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
// Created : 2011-07-16 15:51:46

#ifndef __FX_PRIMITIVETYPEFORWARDINDEXREADER_H
#define __FX_PRIMITIVETYPEFORWARDINDEXREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/ForwardIndexReader.h"
#include "firtex/index/TypedForwardIndexIterator.h"
#include "firtex/thread/FastMutex.h"

FX_NS_DEF(index);

template <typename T>
class PrimitiveTypeForwardIndexReader : public ForwardIndexReader
{
public:
    PrimitiveTypeForwardIndexReader(fdindex_t type);
    ~PrimitiveTypeForwardIndexReader();

public:
    /**
     * Get forward index type
     */
    virtual fdindex_t getType() const { return m_fdIndexType;}

    /// Return value by docId
    const T& getValue(docid_t docId) const;

public:
    /// Return the total size of the forward index
    virtual size_t size() const = 0;

protected:
    /**
     * Load one block into
     * 
     * @param docIdFrom the target docid
     * @param pValues, the loaded values buffer
     * @param docIdTo upper bound doc id of loaded valudes
     * @return false if failure
     */
    virtual bool loadRange(docid_t docIdFrom, const T*& pValues,
                           docid_t& docIdTo) const = 0;

private:
    /// Forward index type
    fdindex_t m_fdIndexType;

    /// Loaded values
    mutable const T* m_pValues;

    mutable FX_NS(thread)::FastMutex m_mutex;

    /// Lower bound docid of loaded values
    mutable docid_t m_docIdFrom;

    /// Upper bound docid of loaded values
    mutable docid_t m_docIdTo;
    
private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////////////////////////
//
SETUP_STREAM_LOGGER_TEMP(index, PrimitiveTypeForwardIndexReader);

template <typename T>
PrimitiveTypeForwardIndexReader<T>::PrimitiveTypeForwardIndexReader(fdindex_t type)
    : m_fdIndexType(type)
    , m_pValues(NULL)
    , m_docIdFrom(INVALID_DOCID)
    , m_docIdTo(INVALID_DOCID)
{
}

template <typename T>
PrimitiveTypeForwardIndexReader<T>::~PrimitiveTypeForwardIndexReader()
{
}

template <typename T>
inline const T& PrimitiveTypeForwardIndexReader<T>::getValue(docid_t docId) const
{
    if (docId >= m_docIdFrom && docId <= m_docIdTo)
    {
        return m_pValues[(size_t)docId];
    }
    
    {
        FX_NS(thread)::FastMutex::Guard g(m_mutex);
        m_docIdFrom = docId;
        if (!loadRange(docId, m_pValues, m_docIdTo))
        {
            FIRTEX_THROW(OutOfRangeException, "Reload forward index data for doc: [%d] FAILED.",
                         docId);
        }
        return m_pValues[(size_t)docId];
    }
}

FX_NS_END

#endif //__FX_PRIMITIVETYPEFORWARDINDEXREADER_H

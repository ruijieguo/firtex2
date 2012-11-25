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
// Created	: 2010-08-10 21:14:38

#ifndef __FX_TYPEDSKIPLISTWRITER_H
#define __FX_TYPEDSKIPLISTWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/index/TypedSkipListReader.h"
#include "firtex/utility/Pool.h"
#include <typeinfo>

FX_NS_DEF(index);

template <typename SkipT>
class TypedSkipListWriter
{
public:
    typedef SkipT SkipPointType;
    typedef TypedSkipListReader<SkipT> ReaderType;

public:
    TypedSkipListWriter(uint32_t nMaxLevels,
                        FX_NS(utility)::Pool* pPostingPool,
                        FX_NS(utility)::Pool* pBufferPool);
    virtual ~TypedSkipListWriter();

public:
    /**
     * Add a skip point
     */
    void addSkipPoint(const SkipPointType& p);

    /**
     * Flush all data in buffer to slice list
     */
    void flush();

    /**
     * Seal the skip-list and release memory of buffers.
     * After sealing, modification is forbidden.
     */
    void seal();

    /**
     * Commit data to output stream
     * @return start offset of skip list
     */
    offset_t commit(FX_NS(store)::OutputStreamPtr& pOutStream);

    /**
     * Create skip list reader
     *
     * @return reader instance
     */
    virtual ReaderType* createReader() const = 0;

    /**
     * Write skip data to byte slice
     * @param level level to write
     * @param sliceWriter slice writer
     * @param pPointers skip pointers array
     * @param pChildPointers child pointer array, only valid when level > 0
     * @param nSize size of array
     * @param bSeal seal the last record if true
     */
    virtual void writeRecord(uint32_t level,
                             ByteSliceOutputStream& sliceWriter,
                             const SkipPointType* pPointers,
                             const offset_t* pChildPointers,
                             uint32_t nSize, bool bSeal) = 0;

public:
    /**
     * Return last value of specified level
     */
    const SkipPointType& getLastFlushedSkip(uint32_t level) const;
    const offset_t getLastFlushedChildOffset(uint32_t level) const;

protected:
    void allocBuffers(uint32_t level);
    void writeRecord(uint32_t level);
    void addSkipPoint(uint32_t level, const SkipPointType& p, offset_t nChildOff);

protected:
    ByteSliceOutputStream** m_ppSkipLevelsStream;
    FX_NS(utility)::Pool* m_pBufferPool;
    SkipPointType** m_ppSkipBuffer;
    offset_t** m_ppChildOffsets;
    uint32_t* m_pVisitsInBuffer;
    
    SkipPointType* m_pLastFlushedSkip;
    offset_t* m_pLastFlushedChildOffset;
    uint32_t m_nMaxSkipLevels;
    uint32_t m_nNumPointsInLowestLevel;

private:
    DECLARE_STREAM_LOGGER();
};

///////////////////////////////////////////////////
//
SETUP_STREAM_LOGGER_TEMP(index, TypedSkipListWriter);

template <typename V>
TypedSkipListWriter<V>::TypedSkipListWriter(uint32_t nMaxLevels,
        FX_NS(utility)::Pool* pPostingPool, FX_NS(utility)::Pool* pBufferPool)
    : m_pBufferPool(pBufferPool)
    , m_nMaxSkipLevels(nMaxLevels)
    , m_nNumPointsInLowestLevel(0)
{
    m_ppSkipLevelsStream = new ByteSliceOutputStream*[nMaxLevels];
    for (uint32_t i = 0; i < nMaxLevels; ++i)
    {
        m_ppSkipLevelsStream[i] = new ByteSliceOutputStream(pPostingPool);
    }
    
    m_ppSkipBuffer = new (pBufferPool->allocate(nMaxLevels * sizeof(SkipPointType*))) SkipPointType*[nMaxLevels];
    memset(m_ppSkipBuffer, 0, nMaxLevels * sizeof(SkipPointType*));

    m_pLastFlushedSkip = new (pBufferPool->allocate(nMaxLevels * sizeof(SkipPointType))) SkipPointType[nMaxLevels];
    memset(m_pLastFlushedSkip, 0, nMaxLevels * sizeof(SkipPointType));

    m_ppChildOffsets = new (pBufferPool->allocate((nMaxLevels - 1) * sizeof(offset_t*))) offset_t*[nMaxLevels - 1];
    memset(m_ppChildOffsets, 0, (nMaxLevels - 1) * sizeof(offset_t*));

    m_pLastFlushedChildOffset = new (pBufferPool->allocate((nMaxLevels - 1) * sizeof(offset_t))) offset_t[nMaxLevels - 1];
    memset(m_pLastFlushedChildOffset, 0, (nMaxLevels - 1) * sizeof(offset_t));

    m_pVisitsInBuffer = new (pBufferPool->allocate(nMaxLevels * sizeof(uint32_t))) uint32_t[nMaxLevels];
    memset(m_pVisitsInBuffer, 0, nMaxLevels * sizeof(uint32_t));
}
 
template <typename V>
TypedSkipListWriter<V>::~TypedSkipListWriter()
{
    for (uint32_t i = 0;i < m_nMaxSkipLevels;i++)
    {
        if(m_ppSkipLevelsStream[i])
        {
            delete m_ppSkipLevelsStream[i];
        }				
    }
    delete[] m_ppSkipLevelsStream;
}

template <typename V>
inline void TypedSkipListWriter<V>::addSkipPoint(const SkipPointType& p)
{
    addSkipPoint(0, p, 0);
}

template <typename SkipT>
void TypedSkipListWriter<SkipT>::writeRecord(uint32_t level)
{
    writeRecord(level, *(m_ppSkipLevelsStream[level]), m_ppSkipBuffer[level],
                (level > 0) ? m_ppChildOffsets[level - 1] : NULL, 
                RECORD_SIZE, false);
    if ((level + 1) < m_nMaxSkipLevels)
    {
        offset_t nChildOffset = 0;
        nChildOffset = m_ppSkipLevelsStream[level]->tell();
        addSkipPoint(level + 1, m_ppSkipBuffer[level][m_pVisitsInBuffer[level] - 1],
                     nChildOffset);
    }
}

template <typename SkipT>
void TypedSkipListWriter<SkipT>::addSkipPoint(uint32_t level, const SkipPointType& p, offset_t nChildOff)
{
    if (!m_ppSkipBuffer[level])
    {
        allocBuffers(level);
    }
    if (m_pVisitsInBuffer[level] == RECORD_SIZE)
    {
        SkipPointType tmpSkip = m_ppSkipBuffer[level][m_pVisitsInBuffer[level] - 1];
        offset_t tmpOff = 0;
        
        if (level != 0)
        {
            tmpOff = m_ppChildOffsets[level - 1][m_pVisitsInBuffer[level] - 1];
        }

        writeRecord(level);

        m_pLastFlushedSkip[level] = tmpSkip;
        if (level != 0)
        {
            m_pLastFlushedChildOffset[level - 1] = tmpOff;
        }

        m_pVisitsInBuffer[level] = 0;
    }
    m_ppSkipBuffer[level][m_pVisitsInBuffer[level]] = p;

    if (level != 0)
    {
        //Store child pointers for all levels except the lowest
        m_ppChildOffsets[level - 1][m_pVisitsInBuffer[level]] = nChildOff;
    }
    ++m_pVisitsInBuffer[level];
}

template <typename SkipT>
void TypedSkipListWriter<SkipT>::allocBuffers(uint32_t level)
{
    m_ppSkipBuffer[level] = new (m_pBufferPool->allocate(
                    RECORD_SIZE * sizeof(SkipPointType))) SkipPointType[RECORD_SIZE];

    if (level != 0)
    {
        m_ppChildOffsets[level - 1] = new (m_pBufferPool->allocate(
                        RECORD_SIZE * sizeof(offset_t))) offset_t[RECORD_SIZE];
    }
    m_pVisitsInBuffer[level] = 0;
}

template <typename SkipT>
inline const typename TypedSkipListWriter<SkipT>::SkipPointType& 
TypedSkipListWriter<SkipT>::getLastFlushedSkip(uint32_t level) const
{
    return m_pLastFlushedSkip[level];
}

template <typename SkipT>
inline const offset_t TypedSkipListWriter<SkipT>::getLastFlushedChildOffset(uint32_t level) const
{
    return m_pLastFlushedChildOffset[level - 1];
}

template <typename SkipT>
inline void TypedSkipListWriter<SkipT>::flush()
{
    for (uint32_t level = 0; level < m_nMaxSkipLevels; ++level)
    {
        if (m_pVisitsInBuffer[level] > 0)
        {
            if (level > 0)
            {
                writeRecord(level, *(m_ppSkipLevelsStream[level]),
                        m_ppSkipBuffer[level],
                        m_ppChildOffsets[level - 1], 
                        m_pVisitsInBuffer[level], true);
            }
            else 
            {
                writeRecord(level, *(m_ppSkipLevelsStream[level]),
                        m_ppSkipBuffer[level], NULL, 
                        m_pVisitsInBuffer[level], true);
            }
            m_pVisitsInBuffer[level] = 0;
        }
    }
}

template <typename SkipT>
void TypedSkipListWriter<SkipT>::seal()
{
    if (m_pVisitsInBuffer)
    {
        flush();
        m_pVisitsInBuffer = NULL;
    }
}

template <typename SkipT>
offset_t TypedSkipListWriter<SkipT>::commit(
        FX_NS(store)::OutputStreamPtr& pOutStream)
{
    if (m_pVisitsInBuffer)
    {
        seal();
    }

    offset_t startOff = pOutStream->tell();
    int32_t nMaxLevel = 0;
    for (int32_t level = (int32_t)m_nMaxSkipLevels - 1; level >= 0; --level)
    {
        if (m_ppSkipLevelsStream[level])
        {
            size_t len = (size_t)m_ppSkipLevelsStream[level]->tell();
            if (len > 0)
            {
                if (nMaxLevel == 0)
                {
                    nMaxLevel = level + 1;
                    FX_TRACE("Max skip level: %d", nMaxLevel);
                    pOutStream->writeVInt32((int32_t)nMaxLevel);
                }
                FX_TRACE("skip level: %d, length: %u, write offset: %lld",
                        level, (uint32_t)len, pOutStream->tell());
                pOutStream->writeVInt32((int32_t)len);
                m_ppSkipLevelsStream[level]->copyTo(*(pOutStream.get()));
            }
        }
    }
    return startOff;
}

FX_NS_END

#endif //__FX_TYPEDSKIPLISTWRITER_H

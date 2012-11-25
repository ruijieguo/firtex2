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
// Created	: 2010-08-15 10:44:49

#ifndef __FX_TYPEDVALUESKIPLISTREADER_H
#define __FX_TYPEDVALUESKIPLISTREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStream.h"
#include <math.h>

FX_NS_DEF(index);

template <typename SkipT>
class TypedSkipListReader
{
public:
    typedef SkipT SkipPointType;

public:
    TypedSkipListReader();
    virtual ~TypedSkipListReader();

public:
    /**
     * Init skip list
     */
    void init(FX_NS(store)::InputStreamPtr& pSkipStream);

    /// Initialize for in-memory skip list
    void init(FX_NS(store)::InputStream** ppInStream, uint32_t nSkipLevels);

    /**
     * Return skip point data of current found skip point.
     */
    const SkipPointType& getCurPoint() const;

    /**
     * Return number of skipped elements
     */
    inline uint32_t getNumSkipped() const;

    /**
     * Read skip data from input stream
     * @param nLevel level to read
     * @param pInStream input stream
     * @param pPointers skip pointers array
     * @param pChildOffsets child offset array, only valid when nLevel > 0
     * @param nSize size of array
     * @param bExhausted set true if encoded records are exhausted
     * @return number of elements has been read
     */
    virtual size_t readRecord(uint32_t nLevel,
                              FX_NS(store)::InputStream* pInStream,
                              SkipPointType* pPointers,
                              offset_t* pChildOffsets,
                              uint32_t nSize, bool& bExhausted) = 0;

    /**
     * Return last skip point of specified level
     */
    const SkipPointType& getLastPoint(uint32_t nLevel) const;

    /**
     * Return last child offset of specified level
     */
    offset_t getLastChildOffset(uint32_t nLevel) const;

    /**
     * Move to next skip in lowest level
     */
    bool moveToNextSkip() { return moveToNextSkip(0);}

protected:
    bool loadNextSkips(uint32_t nLevel);
    bool moveToNextSkip(uint32_t nLevel);
    void seekChild(uint32_t nLevel);
    void setLastSkipData(uint32_t nLevel);
    
protected:
    FX_NS(store)::InputStream** m_ppSkipStreams;
    SkipPointType** m_ppPointsBuffer;
    offset_t** m_ppChildOffsets;
    uint8_t* m_pPointNumInBuffer;
    uint8_t* m_pVisitsInBuffer;
    uint32_t* m_pNumSkipped;
    int32_t* m_pStartOffsets;

    SkipPointType m_curPoint;
    offset_t m_nCurChildOffset;
    uint32_t m_nCurSkipped;

    uint32_t m_nNumSkipLevels;

private:
    DECLARE_STREAM_LOGGER();
};

///////////////////////////////////////////

SETUP_STREAM_LOGGER_TEMP(index, TypedSkipListReader);

template <typename SkipT>
TypedSkipListReader<SkipT>::TypedSkipListReader()
    : m_ppSkipStreams(NULL)
    , m_ppPointsBuffer(NULL)
    , m_ppChildOffsets(NULL)
    , m_pPointNumInBuffer(NULL)
    , m_pVisitsInBuffer(NULL)
    , m_pNumSkipped(NULL)
    , m_pStartOffsets(NULL)
    , m_nCurChildOffset(0)
    , m_nCurSkipped(0)
    , m_nNumSkipLevels(0)
{
}

template <typename SkipT>
TypedSkipListReader<SkipT>::~TypedSkipListReader()
{
    for (uint32_t i = 0; i < m_nNumSkipLevels; ++i)
    {
        delete[] m_ppPointsBuffer[i];
        if (i != 0)
        {
            delete[] m_ppChildOffsets[i - 1];
        }

        if (m_ppSkipStreams[i])
        {
            delete m_ppSkipStreams[i];
        }
    }
    delete[] m_ppSkipStreams;
    delete[] m_ppPointsBuffer;
    delete[] m_ppChildOffsets;
    delete[] m_pPointNumInBuffer;
    delete[] m_pVisitsInBuffer;
    delete[] m_pNumSkipped;
    delete[] m_pStartOffsets;
}

template <typename SkipT>
void TypedSkipListReader<SkipT>::init(
        FX_NS(store)::InputStreamPtr& pSkipStream)
{
    size_t len = 0;
    m_nNumSkipLevels = (uint32_t)pSkipStream->readVInt32();
    FX_TRACE("Start off: %lld, number of skip levels: %u",
                    pSkipStream->tell(), m_nNumSkipLevels);
    if (m_nNumSkipLevels > 0)
    {
        m_ppSkipStreams = new FX_NS(store)::InputStream*[m_nNumSkipLevels];
        m_ppPointsBuffer = new SkipPointType*[m_nNumSkipLevels];
        memset(m_ppPointsBuffer, 0 , m_nNumSkipLevels * sizeof(SkipPointType*));
    
        m_ppChildOffsets = new offset_t*[m_nNumSkipLevels];
        memset(m_ppChildOffsets, 0, m_nNumSkipLevels * sizeof(offset_t*));

        m_pPointNumInBuffer = new uint8_t[m_nNumSkipLevels];
        memset(m_pPointNumInBuffer, 0, m_nNumSkipLevels * sizeof(uint8_t));
    
        m_pVisitsInBuffer = new uint8_t[m_nNumSkipLevels];
        memset(m_pVisitsInBuffer, 0, m_nNumSkipLevels * sizeof(uint8_t));

        m_pNumSkipped = new uint32_t[m_nNumSkipLevels];
        memset(m_pNumSkipped, 0, m_nNumSkipLevels * sizeof(uint32_t));

        m_pStartOffsets = new int32_t[m_nNumSkipLevels];
        memset(m_pStartOffsets, 0, m_nNumSkipLevels * sizeof(int32_t));

        for (uint32_t i = 0; i < m_nNumSkipLevels; ++i)
        {
            m_ppPointsBuffer[i] = new SkipPointType[RECORD_SIZE];
            memset(m_ppPointsBuffer[i], 0, RECORD_SIZE * sizeof(SkipPointType));
            if (i != 0)
            {
                m_ppChildOffsets[i - 1] = new offset_t[RECORD_SIZE];
                memset(m_ppChildOffsets[i - 1], 0, RECORD_SIZE * sizeof(offset_t));
            }
        }
        for (int32_t i = (int32_t)m_nNumSkipLevels - 1; i >= 0; --i)
        {
            m_ppSkipStreams[i] = pSkipStream->clone();
            FX_TRACE("offset: %lld", pSkipStream->tell());
            m_ppSkipStreams[i]->seek(pSkipStream->tell());
            len = (size_t)m_ppSkipStreams[i]->readVInt32();
            FX_TRACE("Skip level: %d, length: %u", i, (uint32_t)len);
            m_pStartOffsets[i] = (int32_t)(m_ppSkipStreams[i]->tell());
            pSkipStream->seek(m_ppSkipStreams[i]->tell() + (offset_t)len);
        }
        for (int32_t j = (int32_t)m_nNumSkipLevels - 1; j >= 0; --j)
        {
            if (!loadNextSkips(j))
            {
                FIRTEX_THROW(IndexCollapseException, "Load skip list FAILED.");
            }
            m_pNumSkipped[j] += (uint32_t)pow((float)SKIP_INTERVAL, (int)j + 1);
        }
    }
}

template <typename SkipT>
void TypedSkipListReader<SkipT>::init(FX_NS(store)::InputStream** ppInStream,
                                      uint32_t nSkipLevels)
{
    m_nNumSkipLevels = nSkipLevels;
    if (m_nNumSkipLevels > 0)
    {
        m_ppSkipStreams = ppInStream;
        m_ppPointsBuffer = new SkipPointType*[m_nNumSkipLevels];
        memset(m_ppPointsBuffer, 0 , m_nNumSkipLevels * sizeof(SkipPointType*));
    
        m_ppChildOffsets = new offset_t*[m_nNumSkipLevels];
        memset(m_ppChildOffsets, 0, m_nNumSkipLevels * sizeof(offset_t*));

        m_pPointNumInBuffer = new uint8_t[m_nNumSkipLevels];
        memset(m_pPointNumInBuffer, 0, m_nNumSkipLevels * sizeof(uint8_t));
    
        m_pVisitsInBuffer = new uint8_t[m_nNumSkipLevels];
        memset(m_pVisitsInBuffer, 0, m_nNumSkipLevels * sizeof(uint8_t));

        m_pNumSkipped = new uint32_t[m_nNumSkipLevels];
        memset(m_pNumSkipped, 0, m_nNumSkipLevels * sizeof(uint32_t));

        m_pStartOffsets = new int32_t[m_nNumSkipLevels];
        memset(m_pStartOffsets, 0, m_nNumSkipLevels * sizeof(int32_t));

        for (uint32_t i = 0; i < m_nNumSkipLevels; ++i)
        {
            m_ppPointsBuffer[i] = new SkipPointType[RECORD_SIZE];
            memset(m_ppPointsBuffer[i], 0, RECORD_SIZE * sizeof(SkipPointType));
            if (i != 0)
            {
                m_ppChildOffsets[i - 1] = new offset_t[RECORD_SIZE];
                memset(m_ppChildOffsets[i - 1], 0, RECORD_SIZE * sizeof(offset_t));
            }
        }

        for (int32_t j = (int32_t)m_nNumSkipLevels - 1; j >= 0; --j)
        {
            if (!loadNextSkips(j))
            {
                FIRTEX_THROW(IndexCollapseException, "Load skip list FAILED.");
            }
            m_pNumSkipped[j] += (uint32_t)pow((float)SKIP_INTERVAL, (int)j + 1);
        }
    }
}

template <typename SkipT>
bool TypedSkipListReader<SkipT>::loadNextSkips(uint32_t nLevel)
{
    if (m_pStartOffsets[nLevel] == -1)
    {
        /// This skip list is exhausted
        return false;
    }
	
    bool bExhausted = false;
    /// read next skips
    m_pPointNumInBuffer[nLevel] = (uint8_t)readRecord(nLevel, m_ppSkipStreams[nLevel],
            m_ppPointsBuffer[nLevel], (nLevel > 0) ? m_ppChildOffsets[nLevel - 1] : NULL,
            RECORD_SIZE, bExhausted);
    m_pVisitsInBuffer[nLevel] = 0;

    if (bExhausted)
    {
        m_pStartOffsets[nLevel] = -1;
    }
    return true;
}

template <typename SkipT>
bool TypedSkipListReader<SkipT>::moveToNextSkip(uint32_t nLevel)
{
    if (m_pStartOffsets[nLevel] != -1 
        || m_pVisitsInBuffer[nLevel] < m_pPointNumInBuffer[nLevel])
    {
        setLastSkipData(nLevel);
        m_pNumSkipped[nLevel] += (uint32_t)pow((float)SKIP_INTERVAL, (int)(nLevel + 1));
    
        ++m_pVisitsInBuffer[nLevel];
        if (m_pVisitsInBuffer[nLevel] >= m_pPointNumInBuffer[nLevel])
        {
            if (!loadNextSkips(nLevel)) //load next skips to buffer
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

template <typename SkipT>
void TypedSkipListReader<SkipT>::seekChild(uint32_t nLevel)
{
    m_ppSkipStreams[nLevel]->seek(m_pStartOffsets[nLevel] + m_nCurChildOffset);
    m_ppPointsBuffer[nLevel][m_pPointNumInBuffer[nLevel] - 1] = m_curPoint;
    if (nLevel > 0)
    {
        m_ppChildOffsets[nLevel - 1][m_pPointNumInBuffer[nLevel] - 1] = m_nCurChildOffset;
    }

    if (!loadNextSkips(nLevel))
    {
        FIRTEX_THROW(IndexCollapseException, "Skip list collapsed.");
    }
    m_pNumSkipped[nLevel] = m_nCurSkipped + (uint32_t)pow((float)SKIP_INTERVAL, (int)nLevel + 1);
}

template <typename SkipT>
void TypedSkipListReader<SkipT>::setLastSkipData(uint32_t nLevel) 
{
    m_curPoint = m_ppPointsBuffer[nLevel][m_pVisitsInBuffer[nLevel]];
    m_nCurSkipped = m_pNumSkipped[nLevel];
    if (nLevel > 0)
    {
        m_nCurChildOffset = m_ppChildOffsets[nLevel - 1][m_pVisitsInBuffer[nLevel]];
//        std::cout << "==setLast: " << nLevel << ", childOffset: " << m_nCurChildOffset << std::endl;
    }
    else 
    {
        m_nCurChildOffset = 0;
    }
}

template <typename SkipT>
inline const typename TypedSkipListReader<SkipT>::SkipPointType&
TypedSkipListReader<SkipT>::getCurPoint() const
{
    return m_curPoint;
}

template <typename SkipT>
inline uint32_t TypedSkipListReader<SkipT>::getNumSkipped() const
{
    return m_nCurSkipped;
}

template <typename SkipT>
const typename TypedSkipListReader<SkipT>::SkipPointType&
TypedSkipListReader<SkipT>::getLastPoint(uint32_t nLevel) const
{
    if (m_pPointNumInBuffer[nLevel] > 0)
    {
        return m_ppPointsBuffer[nLevel][m_pPointNumInBuffer[nLevel] - 1];
    }
    else
    {
        memset(&(m_ppPointsBuffer[nLevel][0]), 0 , sizeof(SkipPointType));
        return m_ppPointsBuffer[nLevel][0];
    } 
}

template <typename SkipT>
offset_t TypedSkipListReader<SkipT>::getLastChildOffset(uint32_t nLevel) const
{
    return (m_pPointNumInBuffer[nLevel] > 0 ? 
            m_ppChildOffsets[nLevel - 1][m_pPointNumInBuffer[nLevel] - 1] : 0);
}

FX_NS_END

#endif //__FX_TYPEDVALUESKIPLISTREADER_H

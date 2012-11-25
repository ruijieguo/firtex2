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
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2006/1/6
//
#include "firtex/index/BarrelTermPositionIterator.h"

FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, BarrelTermPositionIterator);

BarrelTermPositionIterator::BarrelTermPositionIterator()
    : BarrelTermPostingIterator()
    , m_nPosCountInBuffer(0)
    , m_nVisitPosInBuffer((uint32_t)-1)
    , m_nVisitPosInDoc((uint32_t)-1)
    , m_nVisitDocOfPos((uint32_t)-1)
    , m_lastPos(-1)
    , m_nSkippedPosBeforeLastDoc(0)
    , m_nTotalDecodedPos(0)
{
}		

BarrelTermPositionIterator::~BarrelTermPositionIterator(void)
{			
}

void BarrelTermPositionIterator::init(const PostingDecoderPtr& pPosting,
                                      const BitVector* pDocFilter)
{
    BarrelTermPostingIterator::init(pPosting, pDocFilter);
    
    m_nPosCountInBuffer = 0;
    m_nVisitPosInBuffer = (uint32_t)-1;
    m_nVisitPosInDoc = (uint32_t)-1;
    m_nVisitDocOfPos = (uint32_t)0;
    m_lastPos = -1;
    m_nSkippedPosBeforeLastDoc = 0;
    m_nTotalDecodedPos = 0;
}

docid_t BarrelTermPositionIterator::doc()const
{
    return BarrelTermPostingIterator::doc();
}

tf_t BarrelTermPositionIterator::freq()
{
    return BarrelTermPostingIterator::freq();
}

df_t BarrelTermPositionIterator::nextDocs(docid_t*& docs, tf_t*& freqs)
{			
    return BarrelTermPostingIterator::nextDocs(docs, freqs);
}

docid_t BarrelTermPositionIterator::skipTo(docid_t docId)
{
    FX_TRACE("skipTo: %d", docId);

    int64_t curPos = m_nVisitDocInBuffer;
    while (true)
    {
        if (curPos >= m_nDocCountInBuffer)
        {
            if (!decode(docId))
            {
                return INVALID_DOCID;
            }
            curPos = m_nVisitDocInBuffer;
            m_nVisitDocOfPos = (uint32_t)-1;
            m_nVisitPosInDoc = -1;
            m_nSkippedPosBeforeLastDoc = (uint32_t)m_pPostingDecoder->getCumulatedSumTf();
        }

        if (docId <= m_docBuffer[curPos])
        {
            m_nVisitDocInBuffer = (uint32_t)curPos;
            return m_docBuffer[curPos];
        }
        ++curPos;
    }
}

loc_t BarrelTermPositionIterator::skipToPosition(loc_t pos)
{
    FX_TRACE("skip to position: %d, visit doc of pos: %u, visit doc in buffer: %u",
                    pos, m_nVisitDocOfPos, m_nVisitDocInBuffer);
    if (m_nVisitDocOfPos != m_nVisitDocInBuffer)
    {
        moveToCurrentDoc();
    }

    while (pos > m_lastPos)
    {
        if (++m_nVisitPosInDoc >= (uint32_t)m_tfBuffer[m_nVisitDocInBuffer])
        {
            --m_nVisitPosInDoc;
            return INVALID_POS;
        }
        if (++m_nVisitPosInBuffer >= m_nPosCountInBuffer)
        {
            m_nPosCountInBuffer = m_pPostingDecoder->decodePosRecord(
                    (loc_t*)m_posBuffer, m_nTotalDecodedPos, m_nVisitPosInBuffer);

            FX_TRACE("Decode pos record: first pos: %d, length: %u, skip count: %lld, "
                            "visit pos in buffer: %u", m_posBuffer[0], m_nPosCountInBuffer,
                            m_nTotalDecodedPos, m_nVisitPosInBuffer);
            m_nTotalDecodedPos += (m_nPosCountInBuffer - m_nVisitPosInBuffer);
            if (m_lastPos == -1)
            {
                m_lastPos = 0;
            }
        }
        assert(m_nVisitPosInBuffer < RECORD_SIZE);
        m_lastPos += m_posBuffer[m_nVisitPosInBuffer];
    }
    return m_lastPos;
}

tf_t BarrelTermPositionIterator::nextPositions(loc_t*& positions)
{
    if (m_nVisitPosInDoc >= m_nVisitPosInDoc)
    {
        if (m_nVisitPosInDoc >= (uint32_t)m_tfBuffer[m_nVisitDocInBuffer])
        {
            return 0;
        }
        //decodePositions();
    }
//    positions = m_pInDocPosBuffer;
    return m_nVisitPosInDoc;
}	

FX_NS_END


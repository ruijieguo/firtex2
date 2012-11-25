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

#ifndef __FX_BARRELTERMPOSITIONS_H
#define __FX_BARRELTERMPOSITIONS_H

#include "firtex/index/TermPostingIterator.h"
#include "firtex/index/BarrelTermPostingIterator.h"
#include "firtex/index/PostingDecoder.h"
#include <assert.h>

FX_NS_DEF(index);

class BarrelTermPositionIterator : public BarrelTermPostingIterator
{
public:
    BarrelTermPositionIterator();
    virtual ~BarrelTermPositionIterator(void);

public:
    /**
     * Initialize iterator
     */
    void init(const PostingDecoderPtr& pPosting,
              const FX_NS(utility)::BitVector* pDocFilter);

    /**
     * Move to the next documents block
     * @param docs documents block
     * @param freqs term frequency block
     * @return number of documents in the block
     */
    df_t nextDocs(docid_t*& docs, tf_t*& freqs);

    /** 
     * skip to document whose id is equal to or
     *  greater than <i>docId</i>
     */ 
    docid_t skipTo(docid_t docId);

    /**
     * Get current doc id. Only valid after 
     * calling {@link skipTo()} 
     * @return current doc id
     */
    docid_t doc() const;

    /**
     * Get current in document tf. Only valid after 
     * calling {@link skipTo()} 
     * @return current in document tf
     */
    tf_t freq();

    /**
     * Move to next position block
     * @param positions position block
     * @return size of position block
     */
    tf_t nextPositions(loc_t*& positions);

    /**	 
     * Skip to position which is equal to or 
     * greater than <i>pos</i>
     */ 
    loc_t skipToPosition(loc_t pos);

protected:
    inline void moveToCurrentDoc();

protected:			
    loc_t m_posBuffer[RECORD_SIZE];
    uint32_t m_nPosCountInBuffer;
    uint32_t m_nVisitPosInBuffer;
    uint32_t m_nVisitPosInDoc;
    uint32_t m_nVisitDocOfPos;
    loc_t m_lastPos;
    uint32_t m_nSkippedPosBeforeLastDoc;
    ctf_t  m_nTotalDecodedPos;

private:
    DECLARE_STREAM_LOGGER();
};
//////////////////////////////////////////////////////////////////////////
///inline functions
inline void BarrelTermPositionIterator::moveToCurrentDoc()
{
    FX_TRACE("Move to current doc: pos: %d, doc: %d",
             m_nVisitDocInBuffer, m_docBuffer[m_nVisitDocInBuffer]);
    if (m_tfBuffer[0] == 0)
    {
        //Decode corresponding tf record
        uint32_t ret = m_pPostingDecoder->decodeTfRecord((tf_t*)m_tfBuffer);
        FX_TRACE("Decode tf record: first tf: %d, length: %u",
                 m_tfBuffer[0], ret);
        if (ret != m_nDocCountInBuffer)
        {
            FIRTEX_THROW(IndexCollapseException, "Number of decoded docs "
                         "and tfs is not equal.");
        }
//        m_nVisitDocOfPos = 0;
    }

    if (m_nVisitDocOfPos == (uint32_t)-1)
    {
        m_nVisitDocOfPos = 0;
    }

    ctf_t cumSumTf = m_nSkippedPosBeforeLastDoc;
    FX_TRACE("Cumulated sum of tf: %lld, visit doc of pos: %u, "
             "visit doc in buffer: %u, visit pos in doc: %u, "
             "visit pos in buffer: %u", cumSumTf, m_nVisitDocOfPos,
             m_nVisitDocInBuffer, m_nVisitPosInDoc, m_nVisitPosInBuffer);

    int32_t posCount = -(int32_t)(m_nVisitPosInDoc + 1);
    for (size_t i = m_nVisitDocOfPos; i < m_nVisitDocInBuffer; ++i)
    {
        assert(i < RECORD_SIZE);
        posCount += m_tfBuffer[i];
    }
    cumSumTf += (posCount + m_nVisitPosInDoc + 1);
    m_nSkippedPosBeforeLastDoc = (uint32_t)cumSumTf;

    m_nVisitPosInBuffer += (posCount + 1);

    m_nVisitDocOfPos = m_nVisitDocInBuffer;
    m_lastPos = -1;
    m_nVisitPosInDoc = -1;

    FX_TRACE("After moving, Cumulated sum of tf: %lld, visit doc of pos: %u, "
             "visit doc in buffer: %u, visit pos in doc: %u, visit pos in buffer: %u",
             cumSumTf, m_nVisitDocOfPos, m_nVisitDocInBuffer, m_nVisitPosInDoc, m_nVisitPosInBuffer);
    if (cumSumTf >= m_nTotalDecodedPos)
    {
        m_nPosCountInBuffer = m_pPostingDecoder->decodePosRecord(
                (loc_t*)m_posBuffer, cumSumTf, m_nVisitPosInBuffer);

        FX_TRACE("Decode pos record: first pos: %d, length: %u, skip count: %lld, "
                 "visit pos in buffer: %u", m_posBuffer[0], m_nPosCountInBuffer,
                 cumSumTf, m_nVisitPosInBuffer);
        
        m_nTotalDecodedPos = cumSumTf + m_nPosCountInBuffer - m_nVisitPosInBuffer;
    }
    FX_TRACE("skipped pos: %u, visit pos in buffer: %u, total decoded pos: %lld",
             m_nSkippedPosBeforeLastDoc, m_nVisitPosInBuffer, m_nTotalDecodedPos);
    assert(m_nVisitPosInBuffer < RECORD_SIZE);
    m_posBuffer[m_nVisitPosInBuffer] += 1;
    --m_nVisitPosInBuffer;
}

FX_NS_END

#endif

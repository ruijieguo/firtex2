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
// Created	: 2005/12/26
//

#include "firtex/index/BarrelTermPostingIterator.h"
#include "firtex/index/InMemoryTermReader.h"
#include "firtex/utility/Exception.h"

FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, BarrelTermPostingIterator);

BarrelTermPostingIterator::BarrelTermPostingIterator()
    : m_pDocFilter(NULL)
    , m_nDocCountInBuffer(0)
    , m_nVisitDocInBuffer((uint32_t)-1)
{			
}		

BarrelTermPostingIterator::~BarrelTermPostingIterator(void)
{
}

void BarrelTermPostingIterator::init(const PostingDecoderPtr& pPosting,
                                     const BitVector* pDocFilter)
{
    m_pPostingDecoder = pPosting;
    m_pDocFilter = pDocFilter;
    m_nDocCountInBuffer = 0;
    m_nVisitDocInBuffer = (uint32_t)-1;
}

const TermMeta& BarrelTermPostingIterator::getTermMeta() const
{
    return m_pPostingDecoder->getTermMeta();
}

docid_t BarrelTermPostingIterator::doc() const
{
    return (m_nVisitDocInBuffer == (uint32_t)-1) ? 
        INVALID_DOCID : m_docBuffer[m_nVisitDocInBuffer];
}

#define DECODE_TF_RECODE()                                              \
    uint32_t ret = m_pPostingDecoder->decodeTfRecord((tf_t*)m_tfBuffer); \
    if (ret != m_nDocCountInBuffer)                                     \
    {                                                                   \
        FIRTEX_THROW(IndexCollapseException,                            \
                     "Number of decoded docs and tfs is not equal.");   \
    }

tf_t BarrelTermPostingIterator::freq()
{
    if (m_tfBuffer[0] == 0)
    {
        DECODE_TF_RECODE();
    }
    return (m_nVisitDocInBuffer == (uint32_t)-1) ?
        0 : m_tfBuffer[m_nVisitDocInBuffer];
}

df_t BarrelTermPostingIterator::nextDocs(docid_t*& pDocs, tf_t*& pFreqs)
{
    if (m_nVisitDocInBuffer >= m_nDocCountInBuffer)
    {
        if (!decode((m_nDocCountInBuffer > 0) ?
                    m_docBuffer[m_nDocCountInBuffer - 1] + 1 : 0))
        {
            return 0;
        }
        DECODE_TF_RECODE();
    }
    else 
    {
        if (m_tfBuffer[0] == 0)
        {
            DECODE_TF_RECODE();
        }
    }

    pDocs = &m_docBuffer[m_nVisitDocInBuffer];
    pFreqs = &m_tfBuffer[m_nVisitDocInBuffer];
    df_t c = (df_t)(m_nDocCountInBuffer - m_nVisitDocInBuffer);
    m_nVisitDocInBuffer = m_nDocCountInBuffer;
    return c;
}

#undef DECODE_TF_RECODE

docid_t BarrelTermPostingIterator::skipTo(docid_t docId)
{
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
        }

        if (docId <= m_docBuffer[curPos])
        {
            m_nVisitDocInBuffer = (uint32_t)curPos;
            return m_docBuffer[curPos];
        }
        ++curPos;
    }
}

bool BarrelTermPostingIterator::decode(docid_t docId)
{	
    m_nVisitDocInBuffer = 0;
    m_nDocCountInBuffer = m_pPostingDecoder->decodeDocRecord(
            m_docBuffer, docId);
    if (m_pDocFilter && m_pDocFilter->any())
    {
        for (uint32_t i = 0; i < m_nDocCountInBuffer; ++i)
        {
            if (m_pDocFilter->test((size_t)m_docBuffer[i]))
            {
                m_docBuffer[i] = INVALID_DOCID;
            }
        }
    }
    m_tfBuffer[0] = 0;
    if (m_nDocCountInBuffer == 0)
    {
        return false;
    }
    return true;
}

FX_NS_END


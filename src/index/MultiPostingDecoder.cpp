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
// Created	: 2006/1/4
//
#include "firtex/index/MultiPostingDecoder.h"

FX_NS_USE(store);
FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, MultiPostingDecoder);

MultiPostingDecoder::MultiPostingDecoder(void)
    : m_pCurrent(NULL)
{
}

MultiPostingDecoder::~MultiPostingDecoder(void)
{
}

void MultiPostingDecoder::addDecoder(const BarrelInfo* pBarrelInfo,
                                     const PostingDecoderPtr& pPosting)
{
    if (m_postingDecoders.size() > 0
        && m_postingDecoders.back()->m_pBarrelInfo->getBaseDocId() >= pBarrelInfo->getBaseDocId())
    {
        FIRTEX_THROW(OutOfOrderException, "Decoder is out of order.");
    }

    EntryPtr pTmp(new Entry(pBarrelInfo, pPosting));
    m_postingDecoders.push_back(pTmp);
    const TermMeta& termMeta = pPosting->getTermMeta();
    m_termMeta.getCTF() += termMeta.getCTF();
    m_termMeta.getDocFreq() += termMeta.getDocFreq();
}

uint32_t MultiPostingDecoder::decodeDocRecord(docid_t* docBuffer, docid_t docId)
{
    while (m_postingDecoders.size() > 0)
    {
        m_pCurrent = m_postingDecoders.front().get();
        docid_t baseDocId = m_pCurrent->m_pBarrelInfo->getBaseDocId();
        docid_t relDocId = docId - baseDocId;
        uint32_t nDecodedDocs = m_pCurrent->m_pPosting->decodeDocRecord(docBuffer, relDocId);
        if (nDecodedDocs > 0)
        {
            for (uint32_t i = 0; i < nDecodedDocs; ++i)
            {
                docBuffer[i] += baseDocId;
            }
            return nDecodedDocs;
        }
        else
        {
            m_postingDecoders.pop_front();
        }
    }
    return 0;
}

uint32_t MultiPostingDecoder::decodeTfRecord(tf_t* tfBuffer)
{
    return m_pCurrent->m_pPosting->decodeTfRecord(tfBuffer);
}

uint32_t MultiPostingDecoder::decodePosRecord(loc_t* posBuffer, 
        ctf_t nSkipPosCount, uint32_t& nStartOff)
{
    return m_pCurrent->m_pPosting->decodePosRecord(posBuffer, 
            nSkipPosCount, nStartOff);
}

// bool MultiPostingDecoder::hasDeletions()
// {
//     MultiPostingDecoder::Entry* pEntry;			
//     DecoderVector::iterator iter = m_postingDecoders.begin();
//     while (iter != m_postingDecoders.end())
//     {
//         pEntry = (*iter);
//         if(pEntry->isEnable() && pEntry->m_pPosting->hasDeletions())
//             return true;
//         iter++;
//     }	
//     return false;
// }

FX_NS_END


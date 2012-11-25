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
#include "firtex/index/MultiTermPostingIterator.h"
#include "firtex/index/BarrelTermPostingIterator.h"
#include "firtex/index/MultiPostingDecoder.h"

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, MultiTermPostingIterator);

MultiTermPostingIterator::MultiTermPostingIterator(void)
    : m_pCurrent(NULL)
{
}

MultiTermPostingIterator::~MultiTermPostingIterator(void)
{
}

void MultiTermPostingIterator::add(const BarrelInfo* pBarrelInfo,
                                   const TermPostingIteratorPtr& pTermDocs)
{
    m_iterators.push_back(new MultiTermPostingIterator::Entry(pBarrelInfo, pTermDocs));
    if(m_pCurrent == NULL)
    {
        m_pCurrent = m_iterators.front().get();
    }
}

void MultiTermPostingIterator::init()
{
    MultiPostingDecoder* pMultiDecoder = new MultiPostingDecoder();
    m_pPostingDecoder = pMultiDecoder;

    m_pPostingIterQueue = new Queue(m_iterators.size());
    for (PostingIteratorList::iterator iter = m_iterators.begin();
         iter != m_iterators.end(); iter++)
    {
        EntryPtr& pEntry = *iter;
        if (pEntry->postingIterator->skipTo(0) != INVALID_DOCID)
        {
            m_pPostingIterQueue->insert(pEntry.get());
        }

        PostingDecoderPtr pDecoder = pEntry->postingIterator->getPostingDecoder();
        pMultiDecoder->addDecoder(pEntry->barrelInfo, pDecoder);
        const TermMeta& termMeta = pDecoder->getTermMeta();
        m_termMeta.getDocFreq() += termMeta.getDocFreq();
        m_termMeta.getCTF() += termMeta.getCTF();
    }		
    if (m_pPostingIterQueue->size() > 0)
    {
        m_pCurrent = m_pPostingIterQueue->top();
    }
    else 
    {
        m_pCurrent = NULL;
    }
}

const TermMeta& MultiTermPostingIterator::getTermMeta() const
{
    return m_termMeta;
}

df_t MultiTermPostingIterator::nextDocs(docid_t*& docs, tf_t*& freqs)
{
    while (m_pPostingIterQueue->size() > 0)
    {
        m_pCurrent = m_pPostingIterQueue->top();
        df_t ret = m_pCurrent->postingIterator->nextDocs(docs, freqs);
        if (ret> 0)				
        {
            docid_t nBaseId = m_pCurrent->barrelInfo->getBaseDocId();
            if (nBaseId > 0)
            {
                for (df_t i = 0; i < ret; i++)
                {
                    m_docBuffer[i] = docs[i] + nBaseId;
                }
                docs = m_docBuffer;
            }	
            return ret;
        }
        else
        {
            m_pPostingIterQueue->pop();
        }
    }
    return 0;
}

docid_t MultiTermPostingIterator::skipTo(docid_t docId)
{
    while (m_pPostingIterQueue->size() > 0)
    {
        m_pCurrent = m_pPostingIterQueue->top();
        docid_t nBaseId = m_pCurrent->barrelInfo->getBaseDocId();
        docid_t relDocId = docId - nBaseId;

        TermPostingIteratorPtr& pTop = m_pCurrent->postingIterator;
        docid_t retDocId = pTop->skipTo(relDocId);
        if (retDocId >= relDocId)
        {
            return retDocId + nBaseId;
        }
        else 
        {
            m_pPostingIterQueue->pop();
        }
    }	
    return INVALID_DOCID;
}
	
docid_t MultiTermPostingIterator::doc() const
{
    return m_pCurrent->barrelInfo->getBaseDocId() 
        + m_pCurrent->postingIterator->doc();
}

tf_t MultiTermPostingIterator::freq()
{
    return m_pCurrent->postingIterator->freq();
}

tf_t MultiTermPostingIterator::nextPositions(loc_t*& positions)
{
    return m_pCurrent->postingIterator->nextPositions(positions);
}

loc_t MultiTermPostingIterator::skipToPosition(loc_t pos)
{
    return m_pCurrent->postingIterator->skipToPosition(pos);
}

PostingDecoderPtr MultiTermPostingIterator::getPostingDecoder()
{
    return m_pPostingDecoder;
}

FX_NS_END


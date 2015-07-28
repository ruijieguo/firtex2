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
// Created	: 2006/7/20
//
#include "firtex/index/MultiTermIterator.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/index/MultiTermPostingIterator.h"
#include "firtex/index/MultiPostingDecoder.h"
#include "firtex/index/TermInfo.h"

FX_NS_USE(utility);

FX_NS_DEF(index)

SETUP_STREAM_LOGGER(index, MultiTermIterator);

MultiTermIterator::MultiTermIterator(void)
{
}

MultiTermIterator::~MultiTermIterator(void)
{
}

void MultiTermIterator::init()
{
    if (m_pItersQueue || m_iterators.size() == 0)
    {
        return ;
    }
    m_entryHolder.reserve(m_iterators.size());

    m_pItersQueue.reset(new IteratorQueue(m_iterators.size()));

    for (IteratorVector::iterator iter = m_iterators.begin();
         iter != m_iterators.end(); ++iter)
    {
        m_entryHolder.push_back((*iter).get());
    }
}

bool MultiTermIterator::hasNext()
{
    if (m_pItersQueue && (m_entryHolder.size() > 0))
    {
        for (size_t i = 0; i < m_entryHolder.size(); ++i)
        {
            Entry* pTmpEntry = m_entryHolder[i];
            if (pTmpEntry->next())
            {
                m_pItersQueue->put(pTmpEntry);
            }
        }
        m_entryHolder.clear();

        return m_pItersQueue->size() > 0;
    }
    return false;
}
    
TermIterator::TermEntry MultiTermIterator::next()
{
    MultiTermPostingIterator* pMultiPostIter = new MultiTermPostingIterator();
    
    TermPostingIteratorPtr pPostIter(pMultiPostIter);

    Entry* pCurrent = m_pItersQueue->pop();
    m_entryHolder.push_back(pCurrent);

    Entry* pTop = m_pItersQueue->top();
    while (pTop != NULL && pCurrent->termEntry.term->compare(pTop->termEntry.term) == 0)
    {
        m_entryHolder.push_back(m_pItersQueue->pop());
        pTop = m_pItersQueue->top();
    }

    std::sort(m_entryHolder.begin(), m_entryHolder.end(), SORT_CMP);
    for (size_t i = 0; i < m_entryHolder.size(); ++i)
    {
        Entry* pTmpEntry = m_entryHolder[i];
        pMultiPostIter->add(pTmpEntry->barrelInfo,
                            pTmpEntry->termEntry.postingIterator);
    }

    pMultiPostIter->init();
    return TermIterator::TermEntry(pCurrent->termEntry.term, pPostIter);
}

int64_t MultiTermIterator::size() const
{
    int64_t nSize = 0;
    for (IteratorVector::const_iterator iter = m_iterators.begin();
         iter != m_iterators.end(); ++iter)
    {
        nSize += (*iter)->termIterator->size();
    }
    return nSize;
}

void MultiTermIterator::addIterator(const BarrelInfo* pBarrelInfo,
                                    const TermIteratorPtr& iter)
{
    if (m_iterators.size() > 0
        && m_iterators.back()->barrelInfo->getBaseDocId() >= pBarrelInfo->getBaseDocId())
    {
        FIRTEX_THROW(OutOfOrderException, "Term iterator is out of order: adding: "
                     "[%d], last: [%d]", m_iterators.back()->barrelInfo->getBaseDocId(), 
                     pBarrelInfo->getBaseDocId());
    }

    EntryPtr pTmp(new Entry(pBarrelInfo, iter));
    m_iterators.push_back(pTmp);
}

FX_NS_END


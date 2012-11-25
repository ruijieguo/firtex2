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
// Created	: 2006/12/20
//
#include "firtex/index/FieldMerger.h"
#include "firtex/utility/Exception.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/index/IndexBarrelWriter.h"

FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, FieldMerger);

FieldMerger::FieldMerger()
    : m_pFieldSchema(NULL)
    , m_nMergedTerms(0)
{
}

FieldMerger::~FieldMerger(void)
{
}

void FieldMerger::init(const FileSystemPtr& pFileSys,
                       const FieldSchema* pFieldSchema)                       
{
    m_pFileSys = pFileSys;
    m_pFieldSchema = pFieldSchema;
}

void FieldMerger::merge(const IndexMergeInfos& mergeFieldInfos, 
                        FieldMeta& fieldMeta)
{
    beginMerge(mergeFieldInfos);

    if (initQueue(mergeFieldInfos) == false)
    {
        return endMerge();
    }

    int64_t nDistTermCount = 0;

    MergingTermVector match;
    match.reserve(m_pMergeQueue->size());

    const Term* pTerm = NULL;
    MergingTerm* pTop = NULL;

    while (m_pMergeQueue->size() > 0) 
    {
        //Pop the first 
        match.clear();
        match.push_back(m_pMergeQueue->pop());
        pTerm = match[0]->getTerm();
        //Get the current top of the queue
        pTop = m_pMergeQueue->top();

        while (pTop != NULL && (pTerm->compare(pTop->getTerm()) == 0))
        {
            //One match has been found
            match.push_back(m_pMergeQueue->pop());
            //Get the next MergingTerm
            pTop = m_pMergeQueue->top();
        }

        mergeTerms(match);
        nDistTermCount++;

        size_t nMatch = match.size();
        while (nMatch > 0)
        {
            pTop = const_cast<MergingTerm*>(match[--nMatch]);

            //Move to the next term i
            if (pTop->next())
            {
                //There are still some terms
                m_pMergeQueue->put(pTop);
            }
            else
            {
                //No terms anymore
                delete pTop;
            }
        }
    }

    fieldMeta.distinctTermCount = nDistTermCount;
    int64_t nTotalTermCount = 0;
    for (size_t i = 0; i < mergeFieldInfos.size(); ++i)
    {
        const IndexMergeInfo& info = mergeFieldInfos[i];
        nTotalTermCount += info.getBarrelInfo().getIndexMeta().fieldMeta(
                m_pFieldSchema->getName()).totalTermCount;
    }
    fieldMeta.totalTermCount = nTotalTermCount;

    return endMerge();
}

bool FieldMerger::initQueue(const IndexMergeInfos& mergeFieldInfos)
{
    FIRTEX_ASSERT((m_pMergeQueue == NULL),
                  _T("Merge queue has been initialized."));
    if (mergeFieldInfos.size() <= 0)
    {
        return false;
    }

    m_pMergeQueue = new FieldMergeQueue(mergeFieldInfos.size());

    for (size_t i = 0; i < mergeFieldInfos.size(); ++i)
    {
        const IndexMergeInfo& fieldInfo = mergeFieldInfos[i];

        MergingTerm* pMergeTerm = new MergingTerm(fieldInfo);
        if (pMergeTerm->next()) ///get first term
        {
            m_pMergeQueue->put(pMergeTerm);
        }
        else
        {
            delete pMergeTerm;
        }
    }

    return (m_pMergeQueue->size() > 0);
}

FX_NS_END


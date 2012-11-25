//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010/2/3
//
#include "firtex/index/primarykey/PrimaryKeyMerger.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/index/IndexBarrelWriter.h"
#include "firtex/index/FieldComponentCreator.h"
#include <limits>

FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PrimaryKeyMerger);

PrimaryKeyMerger::PrimaryKeyMerger()
    : m_postingTable(std::numeric_limits<uint64_t>::max(), INVALID_DOCID)
{
}

PrimaryKeyMerger::~PrimaryKeyMerger()
{
}

void PrimaryKeyMerger::beginMerge(const IndexMergeInfos& mergeFieldInfos)
{
    m_pPkOutStream = getFileSystem()->createFile(
            BarrelDirectory::getFilePath(getFieldSchema()->getName(), PRIMARYKEY_FILEEXT,
                    mergeFieldInfos.getSuffix()));
}

void PrimaryKeyMerger::merge(const IndexMergeInfos& mergeFieldInfos, 
                             FieldMeta& fieldMeta)
{
    beginMerge(mergeFieldInfos);
    for (size_t i = 0; i < mergeFieldInfos.size(); ++i)
    {
        const IndexMergeInfo& mergeFieldInfo = mergeFieldInfos[i];

        TermReaderPtr pTermReader = mergeFieldInfo.getTermReader();
        PrimaryKeyTermReaderPtr pPkTermReader = pTermReader.cast<PrimaryKeyTermReader>();
        FIRTEX_ASSERT2(!pPkTermReader.isNull());

        mergeWith(mergeFieldInfo, pPkTermReader->getPostingTable());
    }

    fieldMeta.distinctTermCount = m_postingTable.size();
    endMerge();
}

void PrimaryKeyMerger::endMerge()
{
    m_postingTable.save(m_pPkOutStream);

    m_pPkOutStream->close();
    m_pPkOutStream.reset();
}

void PrimaryKeyMerger::mergeWith(const IndexMergeInfo& mergeFieldInfo,
                                 const PostingTablePtr& postingTable)
{
    const DocIdRecycling* pDocIdRecy = mergeFieldInfo.getDocIdRecycling();
    docid_t newBaseId = mergeFieldInfo.getNewBaseDocId();
    PostingTable::Iterator it = postingTable->iterator();
    if (pDocIdRecy && pDocIdRecy->hasDeletions())
    {
        while (it.hasNext())
        {
            PostingTable::KeyValueType& kv = it.next();
            docid_t docId = (*pDocIdRecy)[kv.second];
            if (docId != INVALID_DOCID)
            {
                m_postingTable.insert(make_pair(kv.first, docId + newBaseId));
            }
        }
    }
    else
    {
        while (it.hasNext())
        {
            PostingTable::KeyValueType& kv = it.next();
            m_postingTable.insert(make_pair(kv.first, kv.second + newBaseId));
        }
    }
}

FX_NS_END


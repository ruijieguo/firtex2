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
// Created	: 2005/4/12
//

#include "firtex/index/IndexReader.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/Index.h"
#include "firtex/index/CommitList.h"
#include "firtex/index/MultiIndexBarrelReader.h"
#include "firtex/index/IndexWriterNotification.h"
#include "firtex/utility/NumberFormatter.h"

FX_NS_USE(store);
FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(analyzer);
FX_NS_USE(common);

FX_NS_DEF(index)

SETUP_STREAM_LOGGER(index, IndexReader);

IndexReader::IndexReader()
{
}

IndexReader::IndexReader(const IndexReader& src)
    : m_pKeeper(src.m_pKeeper)
{
}

IndexReader::~IndexReader()
{
    close();
}

IndexReader* IndexReader::clone() const
{
    return new IndexReader(*this);
}

void IndexReader::open(const IndexBarrelKeeperPtr& pBarrelKeeper)
{
    m_pKeeper = pBarrelKeeper;
    m_pBarrel = m_pKeeper->getLatestBarrel();
}

void IndexReader::reopen()
{
    m_pBarrel = m_pKeeper->getLatestBarrel();
}

bool IndexReader::isDeleted(docid_t nDocId) const
{
    return m_pBarrel->getDeletedDocFilter()->isDeleted(nDocId);
}

bool IndexReader::hasDeletions() const
{
    return m_pBarrel->getDeletedDocFilter()->hasDeletions();
}

df_t IndexReader::getDeletedDocCount()const
{
    return m_pBarrel->getDeletedDocFilter()->getDeletedDocCount();
}

df_t IndexReader::getDocCount()const
{
    BarrelsInfoPtr pBarrelsInfo = m_pBarrel->getBarrelsInfo();

    return pBarrelsInfo->getDocCount() - pBarrelsInfo->getDeletedDocCount();
}

void IndexReader::close()
{
    m_pBarrel.reset();
}				

Timestamp IndexReader::lastModified()
{
    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();
    CommitList commits;
    commits.load(pFileSys);
    commitid_t lastCommit = commits.getLastCommit();
    if (lastCommit != INVALID_COMMIT)
    {
        string sComFile = BARREL_COMMIT_FILENAME_PREFIX;
        NumberFormatter::append(sComFile, lastCommit);
        return pFileSys->fileModified(sComFile);
    }
    return Timestamp(0);
}

tstring IndexReader::getIndexVersion() const
{
    BarrelsInfoPtr pBarrelsInfo = m_pBarrel->getBarrelsInfo();
    FIRTEX_ASSERT((pBarrelsInfo != NULL), _T("The index has not opened yet."));
    return pBarrelsInfo->getIndexVersion();
}

df_t IndexReader::getMaxDocs()const
{
    BarrelsInfoPtr pBarrelsInfo = m_pBarrel->getBarrelsInfo();

    FIRTEX_ASSERT((pBarrelsInfo != NULL), _T("The index has not opened yet."));

    return pBarrelsInfo->getDocCount();
}

ForwardIndexIteratorPtr IndexReader::forwardIndexReader(const tstring& sField) const
{
    return m_pBarrel->getReader()->forwardIndexReader(sField);
}

ForwardIndexIteratorPtr IndexReader::forwardIndexReader(fieldid_t fieldId) const
{
    const FieldSchema* pFieldSchema = m_pKeeper->getDocSchema()->getSchema(fieldId);
    if (!pFieldSchema)
    {
        return ForwardIndexIteratorPtr();
    }
    return forwardIndexReader(pFieldSchema->getName());
}

StoredFieldsReaderPtr IndexReader::createStoredFieldsReader() const
{
    return m_pBarrel->getReader()->createStoredFieldsReader();
}


void IndexReader::update(const Notification& notif)
{
    //TODO:
}

IndexMeta IndexReader::getIndexMeta() const
{
    BarrelsInfoPtr pBarrelsInfo = m_pBarrel->getBarrelsInfo();

    if (pBarrelsInfo->getBarrelCount() == 1)
    {
        return pBarrelsInfo->getLastBarrel().getIndexMeta();
    }

    IndexMeta indexMeta;
    for (size_t i = 0; i < pBarrelsInfo->getBarrelCount(); ++i)
    {
        const BarrelInfo& barrelInfo = (*pBarrelsInfo)[i];
        const IndexMeta& meta = barrelInfo.getIndexMeta();
        for (size_t j = 0; j < meta.size(); ++j)
        {
            const FieldMeta& fieldMeta = meta[j];
            FieldMeta& fieldMeta2 = indexMeta.fieldMeta(fieldMeta.fieldName);
            fieldMeta2.totalTermCount += fieldMeta.totalTermCount;
            fieldMeta2.distinctTermCount += fieldMeta.distinctTermCount;
        }
    }
    return indexMeta;
}

FX_NS_END


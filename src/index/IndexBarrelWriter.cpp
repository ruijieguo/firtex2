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
// Created	: 2005/11/27
//

#include "firtex/common/StdHeader.h"
#include "firtex/store/OutputStream.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/index/ComponentBuilder.h"
#include "firtex/index/IndexBarrelKeeper.h"
#include "firtex/index/InMemIndexBarrelReader.h"
#include "firtex/index/IndexBarrelWriter.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/config/GlobalConfig.h"
#include <sstream>
#include <fstream>

using namespace std;
FX_NS_USE(store);
FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(analyzer);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_LOGGER(index, IndexBarrelWriter);

IndexBarrelWriter::IndexBarrelWriter(IndexBarrelKeeper* pBarrelKeeper,
                                     commitid_t cachedBarrelId)
    : m_pKeeper(pBarrelKeeper)
    , m_state(STATE_UNSEALED)
    , m_nUncommittedDocs(0)
{
    m_pLengthNormWriter = new LengthNormWriter(m_pKeeper->getFileSystem());
    m_pTokenSource = new FX_NS(analyzer)::TokenSource();

    m_barrelInfo.setCommitId(cachedBarrelId);
    m_barrelInfo.setState(BarrelInfo::BUILDING);
}

IndexBarrelWriter::~IndexBarrelWriter()
{
    m_indexers.clear();

    m_pPostingPool.reset();

    cleanCacheDir();
}

bool IndexBarrelWriter::init(const SyncSegregatedAllocatorPtr& pAllocator)
{
    m_pBoundedAllocator = new BoundedChunkAllocator(pAllocator);
    if (!m_pBoundedAllocator->init(MAX_CHUNK_COUNT_PER_SEGMENT))
    {
        return false;
    }

    m_pPostingPool = new PostingPool(new LooseBoundedPool(m_pBoundedAllocator),
            new LooseBoundedPool(m_pBoundedAllocator));

    //Init stored field writer
    m_pStoredFieldsWriter = new StoredFieldsWriter(
            m_pKeeper->getFileSystem(), m_barrelInfo.getSuffix());
    const DocumentSchema* pDocSchema = m_pKeeper->getDocSchema();
    m_pAnalyzedDoc = new AnalyzedDocument(pDocSchema);

    //Init index/forward index
    DocumentSchema::Iterator iter = pDocSchema->iterator();
    while (iter.hasNext())
    {
        const FieldSchema* pFieldSchema = iter.next();
        fieldid_t fieldId = pFieldSchema->getId();
        if (pFieldSchema->isIndexed())
        {
            addIndex(fieldId);
        }

        if (pFieldSchema->hasForwardIndex())
        {
            addForwardIndex(fieldId);
        }
    }
    m_pLengthNormWriter->init(pDocSchema);
    m_pInMemDocFilter = new BitVector();

    string sCacheDir = m_barrelInfo.getSuffix();
    FX_DEBUG("Create cache directory: [%s]", sCacheDir.c_str());
    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();
    BarrelDirectory::createBarrel(pFileSys, sCacheDir);

    return true;
}

void IndexBarrelWriter::addIndex(fieldid_t fieldId)
{
    if (fieldId >= (fieldid_t)m_indexers.size())
    {
        m_indexers.resize((size_t)fieldId + 1);
    }

    const FieldSchema* pFieldSchema = m_pKeeper->getDocSchema()->getSchema(fieldId); 
    FIRTEX_ASSERT2(pFieldSchema);
    FieldIndexer* pIndexer = m_pKeeper->getComponentBuilder()->buildIndexer(fieldId, pFieldSchema);
    if (pIndexer == NULL)
    {
        FIRTEX_THROW(InvalidConfigException, "Create indexer of field: [%s] FAILED.",
                     pFieldSchema->getName().c_str());
    }

    if (!strCompareNoCase(pFieldSchema->getTypeName().c_str(), "PRIMARY_KEY"))
    {
        PrimaryKeyIndexer* pPkIndexer = dynamic_cast<PrimaryKeyIndexer*>(pIndexer);
        FIRTEX_ASSERT2(pPkIndexer);

        BarrelsInfoPtr pEmptyBarrelsInfo(new BarrelsInfo);
        pEmptyBarrelsInfo->newBarrel();
        m_pPrimaryKeyIndex = new PrimaryKeyIndex(pFieldSchema->getName());
        m_pPrimaryKeyIndex->open(m_pKeeper->getFileSystem(),
                pEmptyBarrelsInfo, pPkIndexer->getPostingTable());
    }

    m_indexers[fieldId].assign(pIndexer);

    pIndexer->init(m_pPostingPool);
}

void IndexBarrelWriter::addForwardIndex(fieldid_t fieldId)
{
    if (fieldId >= (fieldid_t)m_forwardIndices.size())
    {
        m_forwardIndices.resize(fieldId + 1);
    }
            
    ForwardIndexWriter* pFDIndex = m_pKeeper->getComponentBuilder()->buildForwardIndexWriter(fieldId);
    if (pFDIndex == NULL)
    {
        FIRTEX_THROW(InvalidConfigException, "Create forward index of "
                     "field [%d] FAILED.", fieldId);
    }
    pFDIndex->init(m_pKeeper->getDocSchema()->getSchema(fieldId));
    m_forwardIndices[fieldId].assign(pFDIndex);
}

void IndexBarrelWriter::sealBarrel()
{
    if (m_pPostingPool->bufferPool)
    {
        FX_DEBUG("Seal index barrel");

        m_pStoredFieldsWriter->seal();
        sealIndex();
        sealForwardIndex();

        /// Release memory for posting buffer
        m_pPostingPool->resetBufferPool();
        FX_DEBUG("End seal index barrel");
    }
    m_state = STATE_SEALED;
}

void IndexBarrelWriter::sealIndex()
{
    FX_DEBUG("Begin seal index...");

    for (fieldid_t i = 0; i < (fieldid_t)m_indexers.size(); i++)
    {
        if (m_indexers[i].isNull())
            continue;

        m_indexers[i]->sealIndex();
    }

    FX_DEBUG("End seal index.");

}

void IndexBarrelWriter::sealForwardIndex()
{
    FX_DEBUG("Begin seal forward index...");

    for (fieldid_t i = 0; i < (fieldid_t)m_forwardIndices.size(); i++)
    {
        if (m_forwardIndices[i].isNull())
            continue;
        m_forwardIndices[i]->seal((docid_t)(m_nUncommittedDocs - 1));
    }

    FX_DEBUG("End seal forward index.");

}

void IndexBarrelWriter::commit()
{
    FX_DEBUG("Commit");

    sealBarrel();

    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();

    if (m_nUncommittedDocs > 0)
    {
        BarrelsInfo barrelsInfo;
        barrelsInfo.read(pFileSys);
        barrelsInfo.setEncoding(m_pKeeper->getEncoding());
        BarrelInfo& newBarrel = barrelsInfo.newBarrel();
        newBarrel.setDocCount(m_barrelInfo.getDocCount());
        newBarrel.setDeletedDocs(m_barrelInfo.getDeletedDocCount());
        IndexMeta& indexMeta = newBarrel.getIndexMeta();
        const IndexMeta& indexMeta2 = m_barrelInfo.getIndexMeta();
        indexMeta = indexMeta2;

        std::string sSuffix = newBarrel.getSuffix();
        
        FX_LOG(INFO, "Commit in-memory index barrel: [%s], doc count[%u]",
               sSuffix.c_str(), m_nUncommittedDocs);
        BarrelDirectory::createBarrel(pFileSys, sSuffix);
        commitIndex(sSuffix);
        commitForwardIndex(sSuffix);
        commitDeletion(newBarrel);

        m_pLengthNormWriter->commitBarrel(sSuffix);
        m_pStoredFieldsWriter->commitBarrel(sSuffix);
        
        cleanCacheDir();
        barrelsInfo.write(pFileSys);

        FX_LOG(INFO, "End commit in-memory index barrel: [%s], doc count[%u]",
               sSuffix.c_str(), m_nUncommittedDocs);

        m_nUncommittedDocs = 0;
        
        m_pKeeper->refresh();

        /// Clean staled index data if possible
        m_pKeeper->cleanStaledIndex();

        return;
    }
    
    if ((m_pInMemDocFilter.isNotNull() && m_pInMemDocFilter->any())
        || m_pDocFilterNeedCommit.isNotNull())
    {
        BarrelsInfo barrelsInfo;
        barrelsInfo.read(pFileSys);
        barrelsInfo.setEncoding(m_pKeeper->getEncoding());
        BarrelInfo& newBarrel = barrelsInfo.newBarrel();
        newBarrel.setDocCount(m_barrelInfo.getDocCount());
        newBarrel.setDeletedDocs(m_barrelInfo.getDeletedDocCount());
        IndexMeta& indexMeta = newBarrel.getIndexMeta();
        const IndexMeta& indexMeta2 = m_barrelInfo.getIndexMeta();
        indexMeta = indexMeta2;
                
        std::string sSuffix = newBarrel.getSuffix();
        FX_LOG(INFO, "Commit in-memory index barrel: [%s], doc count[%u]",
               sSuffix.c_str(), m_nUncommittedDocs);
        BarrelDirectory::createBarrel(pFileSys, sSuffix);
        commitDeletion(newBarrel);

        cleanCacheDir();
        barrelsInfo.write(pFileSys);

        m_pKeeper->refresh();

        /// Clean staled index data if possible
        m_pKeeper->cleanStaledIndex();
        
        FX_LOG(INFO, "End commit in-memory index barrel: [%s], doc count[%u]",
               sSuffix.c_str(), m_nUncommittedDocs);
    }
}

bool IndexBarrelWriter::commitBarrelInfo(BarrelsInfoPtr& pBarrelsInfo)
{
    DeletedDocumentFilterPtr pDocFilter = m_pKeeper->getOnDiskDocFilterUnSafe();
    pBarrelsInfo->setEncoding(m_sEncoding);
    if (m_nUncommittedDocs > 0)
    {
        updateBarrelInfo(pBarrelsInfo);
        
        pDocFilter->appendBarrel(m_barrelInfo, m_pInMemDocFilter);

        BarrelInfo& barrelInfo = pBarrelsInfo->getLastBarrel();
        IndexMeta& indexMeta = barrelInfo.getIndexMeta();
        commitIndexMeta(indexMeta);
        m_barrelInfo = barrelInfo;

		pDocFilter->seal();
        m_pDocFilterNeedCommit = pDocFilter;

        return true;
    }

    if (pDocFilter.isNotNull() && pDocFilter->isDirty() && !pDocFilter->hasSealed())
    {
        updateBarrelInfo(pBarrelsInfo);
        m_barrelInfo.setDocCount(0);

        pDocFilter->seal();

        m_pDocFilterNeedCommit = pDocFilter;

        return true;
    }
    return false;
}

void IndexBarrelWriter::addDocument(const Document* pDoc)
{
    docid_t docId = m_nUncommittedDocs++;

    m_pAnalyzedDoc->clear();
    m_pAnalyzedDoc->setDocId(docId);

    if (m_pPrimaryKeyIndex.isNotNull())
    {
        const char* szPk = pDoc->getPrimaryKey();
        docid_t oldDocId = m_pPrimaryKeyIndex->lookup(szPk);
        if (oldDocId != INVALID_DOCID)
        {
            m_pPrimaryKeyIndex->replace(szPk, docId);
            m_pInMemDocFilter->set(oldDocId);
        }
        else
        {
            m_pPrimaryKeyIndex->addPrimaryKey(szPk, docId);
        }
    }

    Document::Iterator iter = pDoc->iterator();
    if (iter.size() == 0)
    {
        cout << "------Empty document " << endl;
        return;
    }

    const Field* pField = NULL;
    while (iter.hasNext())
    {
        pField = iter.next();
        fieldid_t fieldId = pField->getId();

        AnalyzedField* pAnalyzedField =
            m_pAnalyzedDoc->addField(pField->getId());
        pAnalyzedField->setField(pField);

        const AnalyzerMapper* pAnalyzerMapper = m_pKeeper->getAnalyzerMapper();
        const FieldSchema* pFieldSchema = pField->getFieldSchema();
        if (pFieldSchema->isAnalyzed())//Analyze field
        {
            const Analyzer* pAnalyzer = 
                pAnalyzerMapper->getAnalyzer(fieldId);
            if (!pAnalyzer)
            {
                FIRTEX_THROW(AnalyzeException, "No analyzer for field: [%s]",
                        pFieldSchema->getName().c_str());
            }
            
            const Field::String& text = pField->getValue();
            if (text.length() > 0)
            {
                m_pTokenSource->reset();
                m_pTokenSource->setOriginalView(text.c_str(), text.length());
                TokenViewPtr pTokenView = pAnalyzer->tokenize(m_pTokenSource);

                pAnalyzedField->setTokenView(pTokenView);
            }
        }

        if (pFieldSchema->isIndexed())
        {
            FIRTEX_ASSERT2(m_indexers[fieldId] != NULL);
            m_indexers[fieldId]->addField(pAnalyzedField);

            if (pFieldSchema->hasLengthNorm())
            {
                m_pLengthNormWriter->addField(docId, pAnalyzedField);
            }
        }
        
        if (pFieldSchema->hasForwardIndex())
        {
            FIRTEX_ASSERT2(fieldId < (fieldid_t)m_forwardIndices.size() 
                           && m_forwardIndices[fieldId] != NULL);
            m_forwardIndices[fieldId]->addField(docId, pAnalyzedField);
        }
    }

    m_pStoredFieldsWriter->addDocument(m_pAnalyzedDoc.get());

    for (size_t i = 0; i < m_indexers.size(); ++i)
    {
        if (m_indexers[i].isNotNull())
        {
            m_indexers[i]->commitDocument(docId);
        }
    }
}

bool IndexBarrelWriter::deleteDocument(const FX_NS(document)::Document* pDoc)
{
    const char* szPk = pDoc->getPrimaryKey();
    if(szPk && m_pPrimaryKeyIndex.isNotNull())
    {
        FX_TRACE("Try delete in-memory document: [%s]", szPk);
        docid_t docId = m_pPrimaryKeyIndex->lookup(szPk);
        if (docId != INVALID_DOCID)
        {
            m_pInMemDocFilter->set(docId);
            FX_DEBUG("Delete in-memory document: [%s]->[%d] success.", 
                     szPk, docId);

            return true;
        }
    }
    return false;
}

void IndexBarrelWriter::undeleteAll()
{
    m_pInMemDocFilter->reset();
}

void IndexBarrelWriter::commitIndex(const std::string& sSuffix)
{
    FX_DEBUG("Begin commit index to file system...");

    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();
    for (fieldid_t i = 0; i < (fieldid_t)m_indexers.size(); i++)
    {
        if (m_indexers[i].isNull())
            continue;

        m_indexers[i]->commit(pFileSys, sSuffix);
    }
    FX_DEBUG("End commit index.");
}

void IndexBarrelWriter::commitIndexMeta(IndexMeta& indexMeta)
{
    FX_DEBUG("Begin commit index meta ...");

    const DocumentSchema* pDocSchema = m_pKeeper->getDocSchema();
    for (fieldid_t i = 0; i < (fieldid_t)m_indexers.size(); i++)
    {
        if (m_indexers[i].isNull())
            continue;

        const FieldSchema* pFieldSchema = pDocSchema->getSchema(i);
        FIRTEX_ASSERT2(pFieldSchema);
        FieldMeta& fieldMeta = indexMeta.fieldMeta(pFieldSchema->getName());

        m_indexers[i]->commitMeta(fieldMeta);
    }
    FX_DEBUG("End commit index meta.");
}


void IndexBarrelWriter::commitForwardIndex(const std::string& sSuffix)
{
    if (m_forwardIndices.size() > 0)
    {
        FileSystemPtr pFileSys = m_pKeeper->getFileSystem();
        for (fieldid_t i = 0; i < (fieldid_t)m_forwardIndices.size(); i++)
        {
            if (m_forwardIndices[i].isNull())
            {
                continue;
            }
            (void)m_forwardIndices[i]->commit(pFileSys, FORWARD_INDEX_FILEEXT, sSuffix);
        }    
    }
}

void IndexBarrelWriter::commitDeletion(const BarrelInfo& barrelInfo)
{
    if (m_pDocFilterNeedCommit.isNotNull())
    {
        m_pDocFilterNeedCommit->commit(barrelInfo);
        m_pDocFilterNeedCommit.reset();
    }
}

FieldIndexerPtr IndexBarrelWriter::getFieldIndexer(const std::string& field) const 
{
    fieldid_t fieldId =	m_pKeeper->getDocSchema()->getFieldId(field);
    if (fieldId == INVALID_FIELDID || fieldId >= (fieldid_t)m_indexers.size())
    {
        return FieldIndexerPtr();
    }

    return m_indexers[fieldId];
}

FieldIndexerPtr IndexBarrelWriter::getFieldIndexer(fieldid_t fieldId) const
{
    FIRTEX_ASSERT2(fieldId < (fieldid_t)m_indexers.size());
    return m_indexers[fieldId];
}

ForwardIndexWriterPtr IndexBarrelWriter::getForwardIndexWriter(
        fieldid_t fieldId) const
{
    if (fieldId == INVALID_FIELDID || fieldId >= (fieldid_t)m_forwardIndices.size())
    {
        return ForwardIndexWriterPtr();
    }
    return m_forwardIndices[fieldId];
}

IndexBarrelReader* IndexBarrelWriter::inMemoryReader() const
{
    InMemIndexBarrelReader* pReader = new InMemIndexBarrelReader(this, m_pKeeper);
    return pReader;
}

void IndexBarrelWriter::updateBarrelInfo(BarrelsInfoPtr& pBarrelsInfo)
{
    if (pBarrelsInfo->getBarrelCount() > 0)
    {
        const BarrelInfo& lastBarrel = pBarrelsInfo->getLastBarrel();
        m_barrelInfo.setBaseDocId(lastBarrel.getBaseDocId() + lastBarrel.getDocCount());
    }
    m_barrelInfo.setDocCount((df_t)m_nUncommittedDocs);
    if (m_pInMemDocFilter.isNotNull())
    {
        m_barrelInfo.setDeletedDocs((df_t)m_pInMemDocFilter->count());
    }
    FX_DEBUG("update barrel info: commit: [%d], baseDocId: [%d], docCount: [%d]", 
             m_barrelInfo.getCommitId(), m_barrelInfo.getBaseDocId(), m_barrelInfo.getDocCount());
    
    BarrelInfo& newBarrel = pBarrelsInfo->newBarrel();
    newBarrel.setBaseDocId(m_barrelInfo.getBaseDocId());
    newBarrel.setDocCount(m_barrelInfo.getDocCount());
    newBarrel.setDeletedDocs(m_barrelInfo.getDeletedDocCount());
    newBarrel.setState(BarrelInfo::MERGING);

    string sOldSuffix = m_barrelInfo.getSuffix();
    m_barrelInfo = newBarrel;
    string sNewSuffix = m_barrelInfo.getSuffix();
    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();    
    pFileSys->moveTo(sOldSuffix, sNewSuffix);
    
    m_pStoredFieldsWriter->setCacheDir(m_barrelInfo.getSuffix());
}

void IndexBarrelWriter::cleanCacheDir()
{
    string sCacheDir = m_barrelInfo.getSuffix();

    FileSystemPtr pFileSys = m_pKeeper->getFileSystem();
    if (pFileSys->fileExists(sCacheDir))
    {
        FX_DEBUG("Clean cache dir: [%s]", sCacheDir.c_str());
        pFileSys->deleteDirectory(sCacheDir, true);
    }
}

FX_NS_END


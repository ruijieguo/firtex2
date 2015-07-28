//
// Copyright(C) 2005--2009 Guo Ruijie
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2009/3/14
//
#include "firtex/index/primarykey/PrimaryKeyIndexer.h"
#include "firtex/index/primarykey/PrimaryKeyTermReader.h"
#include "firtex/utility/Hash.h"
#include "firtex/index/BarrelDirectory.h"
#include <limits>

FX_NS_USE(analyzer);
FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PrimaryKeyIndexer);

PrimaryKeyIndexer::PrimaryKeyIndexer(const FieldSchema* pFieldSchema)
    : FieldIndexer(pFieldSchema)
    , m_pPostingTable(new PostingTable(std::numeric_limits<uint64_t>::max(), INVALID_DOCID))
    , m_pCurField(NULL)
{
}

PrimaryKeyIndexer::~PrimaryKeyIndexer(void)
{
    m_pPostingTable.reset();
}

void PrimaryKeyIndexer::init(const PostingPoolPtr& pPostingPool)
{
}

void PrimaryKeyIndexer::addField(const AnalyzedField* pField)
{
    FX_TRACE("Add pk field: [%s]", pField->getField()->getValue().c_str());
    m_pCurField = pField;
}

void PrimaryKeyIndexer::commitDocument(docid_t docId)
{
    commitValue(docId);
}

void PrimaryKeyIndexer::commitValue(docid_t docId)
{
    if (!m_pCurField)
    {
        FX_LOG(ERROR, "Commit document: [%d] without primary key", (int32_t)docId);
        return;
    }
    const AnalyzedField::TokenView* pTokenView = m_pCurField->getTokenView();
    if (pTokenView)
    {
        TokenView::Iterator it = pTokenView->iterator();
        if (it.hasNext())
        {
            const Token& token = it.next();
            uint64_t hashValue = Hash::hashString64(token.getTextValue());
            docid_t did = m_pPostingTable->find(hashValue);
            if (did == INVALID_DOCID)
            {
                FX_TRACE("commit pk field: [%s]->[%d]", token.getTextValue(), docId);
                m_pPostingTable->insert(hashValue, docId);
            }
            else
            {
                FX_LOG(WARN, "Primary key duplicated: [%s]", token.getTextValue());
            }
        }
    }
    else
    {
        const Field* pOrgField = m_pCurField->getField();
        const Field::String& sValue = pOrgField->getValue();
        if (sValue.length() == 0)
        {
            FX_LOG(ERROR, "Commit document: [%d], primary key is empty", (int32_t)docId);
            return;
        }
        uint64_t hashValue = Hash::hashString64(sValue.c_str(), sValue.length());
        docid_t did = m_pPostingTable->find(hashValue);
        if (did == INVALID_DOCID)
        {
            FX_TRACE("commit pk field: [%s]->[%d]", sValue.c_str(), docId);
            m_pPostingTable->insert(hashValue, docId);
        }
        else
        {
            string str(sValue.c_str(), sValue.length());
            FX_LOG(WARN, "Primary key duplicated: [%s]", str.c_str());
        }
    }
    m_pCurField = NULL;
}

void PrimaryKeyIndexer::commit(FileSystemPtr& pFileSys,
                               const std::string& sSuffix)
{
    OutputStreamPtr pPKOutput = pFileSys->createFile(
            BarrelDirectory::getFilePath(getFieldSchema()->getName(), 
                    PRIMARYKEY_FILEEXT, sSuffix));
    m_pPostingTable->save(pPKOutput);

    m_pPostingTable.reset(new PostingTable(
                    std::numeric_limits<uint64_t>::max(), INVALID_DOCID));
}

void PrimaryKeyIndexer::commitMeta(FieldMeta& fieldMeta)
{
    fieldMeta.totalTermCount = fieldMeta.distinctTermCount = 
                               m_pPostingTable->size();
}

void PrimaryKeyIndexer::clear()
{
    m_pPostingTable->clear();
}

uint64_t PrimaryKeyIndexer::distinctNumTerms() const
{
    return (uint64_t)m_pPostingTable->size();
}

TermReaderPtr PrimaryKeyIndexer::termReader() const
{
    return TermReaderPtr(new PrimaryKeyTermReader(getFieldSchema(),
                    m_pPostingTable));
}

FX_NS_END

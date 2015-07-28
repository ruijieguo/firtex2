//
// Copyright(C) 2005--2010 FirteX Develop Team
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010/1/13
//
#include "firtex/index/primarykey/PrimaryKeyTermReader.h"
#include "firtex/index/primarykey/PrimaryKeyIndexer.h"
#include "firtex/index/primarykey/PrimaryKeyTermIterator.h"
#include "firtex/index/primarykey/PrimaryKeyPostingIterator.h"
#include "firtex/index/BarrelDirectory.h"
#include <limits>

FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PrimaryKeyTermReader);

PrimaryKeyTermReader::PrimaryKeyTermReader() 
{
}

PrimaryKeyTermReader::PrimaryKeyTermReader(const PrimaryKeyTermReader& src)
    : TermReader(src)
    , m_pPostingTable(src.m_pPostingTable)
{
}

PrimaryKeyTermReader::PrimaryKeyTermReader(const FieldSchema* pFieldSchema,
        const PostingTablePtr& postingTable)
    : TermReader(pFieldSchema)
    , m_pPostingTable(postingTable)
{
}

PrimaryKeyTermReader::~PrimaryKeyTermReader(void)
{
}

void PrimaryKeyTermReader::open(const FileSystemPtr& pFileSys,
                                const InputStreamPoolPtr& pStreamPool,
                                const std::string& sSuffix, 
                                const FieldSchema* pFieldSchema,
                                const BitVector* pDocFilter)
{
    FIRTEX_ASSERT2(!m_pPostingTable);

    TermReader::open(pFileSys, pStreamPool, sSuffix, pFieldSchema, pDocFilter);

    std::string sSuffixName = sSuffix;
    FileSystemPtr pTmpFileSys = pFileSys;
    InputStreamPtr pInputStream = pTmpFileSys->openFile(
            BarrelDirectory::getFilePath(pFieldSchema->getName(),
                    PRIMARYKEY_FILEEXT, sSuffix));

    m_pPostingTable.reset(new PostingTable(
                    std::numeric_limits<uint64_t>::max(), INVALID_DOCID));
    
    m_pPostingTable->load(pInputStream);
}

void PrimaryKeyTermReader::open(const FileSystemPtr& pFileSys,
                                const InputStreamPoolPtr& pStreamPool,
                                const std::string& sSuffix, 
                                const std::string& sField,
                                const BitVector* pDocFilter)
{
    FIRTEX_ASSERT2(!m_pPostingTable);

    TermReader::open(pFileSys, pStreamPool, sSuffix, NULL, pDocFilter);
    
    std::string sSuffixName = sSuffix;
    FileSystemPtr pTmpFileSys = pFileSys;
    InputStreamPtr pInputStream = pTmpFileSys->openFile(
            BarrelDirectory::getFilePath(sField,
                    PRIMARYKEY_FILEEXT, sSuffix));

    m_pPostingTable.reset(new PostingTable(
                    std::numeric_limits<uint64_t>::max(), INVALID_DOCID));
    
    m_pPostingTable->load(pInputStream);
}

TermIteratorPtr PrimaryKeyTermReader::termIterator(
        const Term* pLowerTerm, const Term* pUpperTerm) const
{
    PrimaryKeyTermIterator* pTmpIter = new PrimaryKeyTermIterator();
    TermIteratorPtr pIterPtr = TermIteratorPtr(pTmpIter);
    pTmpIter->init(m_pPostingTable);
    return pIterPtr;
}

TermIteratorPtr PrimaryKeyTermReader::termIterator(const std::string& field) const
{
    if (getFieldSchema()->getName() != field)
        return TermIteratorPtr();
    PrimaryKeyTermIterator* pTmpIter = new PrimaryKeyTermIterator();
    TermIteratorPtr pIterPtr = TermIteratorPtr(pTmpIter);
    pTmpIter->init(m_pPostingTable);
    return pIterPtr;
}

TermPostingIteratorPtr PrimaryKeyTermReader::seek(const Term* pTerm) const
{
    if (pTerm->getField() != getFieldSchema()->getName())
    {
        return TermPostingIteratorPtr();
    }

    uint64_t hashKey;
    const std::string& sValue = pTerm->stringValue();
    if (sValue.empty())
    {
        const TypedTerm<uint64_t>* pTermX = pTerm->cast<uint64_t>();
        if (!pTermX)
        {
            return TermPostingIteratorPtr();
        }
        hashKey = pTermX->getValue();                
    }
    else 
    {
        hashKey = Hash::hashString64(sValue.c_str());
    }
    docid_t docId = m_pPostingTable->find(hashKey);
    if (docId != INVALID_DOCID)
    {
        TermPostingIteratorPtr pIter(new PrimaryKeyPostingIterator(docId));
        return pIter;
    }
    return TermPostingIteratorPtr();
}

TermReader* PrimaryKeyTermReader::clone() const
{
    return new PrimaryKeyTermReader(*this);
}

FX_NS_END

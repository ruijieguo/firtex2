//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-12-25 09:46:15

#ifndef __FX_TYPEDINDEXER_H
#define __FX_TYPEDINDEXER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/HashMap.h"
#include "firtex/index/TypedDict.h"
#include "firtex/index/FieldIndexer.h"
#include "firtex/analyzer/Token.h"
#include <vector>

FX_NS_DEF(index);

template <typename PostingWriter, bool hasPos>
struct TypedIndexCommitter
{
    typedef PostingWriter PostingWriterType;

    void init(FX_NS(store)::FileSystemPtr& pFileSys,
              const std::string& sFieldName,
              const std::string& sSuffix)
    {
        assert(false);
    }
        
    offset_t commit(PostingWriterType& writer)
    {
        assert(false);
        return 0;
    }
};

template <typename Key, typename Hasher, typename Traits>
class TypedIndexer : public FieldIndexer
{
public:
    const static size_t KEY_INDEX_INTERVAL = 128;

public:
    typedef Traits TraitsType;
    typedef Hasher HasherType;
    typedef typename TraitsType::KeyType KeyType;
    typedef typename TraitsType::PostingWriterType PostingWriterType;
    typedef typename TraitsType::PostingTableType PostingTableType;
    typedef typename TraitsType::PostingTableTypePtr PostingTableTypePtr;
    typedef typename TraitsType::DictType DictType;
    

    const static bool WITH_POSITION = TraitsType::WITH_POSITION;

    typedef std::vector<PostingWriterType*> PostingVector;

public:
    TypedIndexer(const FX_NS(document)::FieldSchema* pFieldSchema);
    virtual ~TypedIndexer();

public:
    /**
     * Initialize
     *
     * @param pPostingPool memory pool for posting
     */
     void init(const PostingPoolPtr& pPostingPool);

    /**
     * Seal the index . After sealing, modification is forbidden
     */
    void sealIndex();

    /**
     * Add a field to index
     * @param pField pField object
     */
    void addField(const FX_NS(document)::AnalyzedField* pField);

    /**
     * Finalize a document
     * @param docId docid of current document
     */
    void commitDocument(docid_t docId);

    /**
     * Finalize index and commit to file system
     *
     * @param pFileSys file system instance
     * @param sSuffix suffix of index file name
     */
    void commit(FX_NS(store)::FileSystemPtr& pFileSys,
                const std::string& sSuffix);

    /**
     * Finalize index and extract index meta
     *
     * @param fieldMeta field meta
     */
    void commitMeta(FieldMeta& fieldMeta);

    /**
     * Return number of distinct terms
     */
    uint64_t distinctNumTerms() const;

    /**
     * Return total number of terms
     */
    uint64_t totalTerms() const;

    /**
     * Clear all resource
     */
    void clear();

protected:
    inline void addToken(const FX_NS(analyzer)::Token& token,
                         loc_t& lastPos);
    inline void addToken(const char* token, size_t len, loc_t& lastPos);
    inline void doAddToken(KeyType key, loc_t lastPos);

protected:
    PostingTableTypePtr m_pPostingTable;
    PostingVector m_modifiedPostings;

    PostingPoolPtr m_pPostingPool;

    uint64_t m_nTotalTerms;

    HasherType m_hasher;

private:
    DECLARE_LOGGER();
};

/////////////////////////////////////////////////////////////////
//
template <typename PostingWriter>
struct TypedIndexCommitter<PostingWriter, true>
{
    void init(FX_NS(store)::FileSystemPtr& pFileSys,
              const std::string& sFieldName,
              const std::string& sSuffix)
    {
        m_pDocStream = pFileSys->createFile(
                BarrelDirectory::getFilePath(sFieldName,
                        DOC_POSTING_FILEEXT, sSuffix));
        m_pPosStream = pFileSys->createFile(
                BarrelDirectory::getFilePath(sFieldName,
                        POS_POSTING_FILEEXT, sSuffix));
    }

    inline offset_t commit(PostingWriter* pWriter)
    {
        return pWriter->commit(m_pDocStream, m_pPosStream);
    }

private:
    FX_NS(store)::OutputStreamPtr m_pDocStream;
    FX_NS(store)::OutputStreamPtr m_pPosStream;
};

template <typename PostingWriter>
struct TypedIndexCommitter<PostingWriter, false>
{
    void init(FX_NS(store)::FileSystemPtr& pFileSys,
              const std::string& sFieldName,
              const std::string& sSuffix)
    {
        m_pDocStream = pFileSys->createFile(
                BarrelDirectory::getFilePath(sFieldName,
                        DOC_POSTING_FILEEXT, sSuffix));
    }

    inline offset_t commit(PostingWriter* pWriter)
    {
        return pWriter->commit(m_pDocStream);
    }

private:
    FX_NS(store)::OutputStreamPtr m_pDocStream;
};

/////////////////////////////////////////////////////////////////
//

SETUP_LOGGER_TEMP3(index, TypedIndexer);

template <typename Key, typename Hasher, typename Traits>
TypedIndexer<Key, Hasher, Traits>::TypedIndexer(const FX_NS(document)::FieldSchema* pFieldSchema)
                         
    : FieldIndexer(pFieldSchema)
    , m_nTotalTerms(0)
{
}

template <typename Key, typename Hasher, typename Traits>
TypedIndexer<Key, Hasher, Traits>::~TypedIndexer() 
{
    clear();
}

template <typename Key, typename Hasher, typename Traits>
void TypedIndexer<Key, Hasher, Traits>::init(const PostingPoolPtr& pPostingPool)
{
    m_pPostingPool = pPostingPool;

    m_pPostingTable.reset(new PostingTableType(m_pPostingPool->postingPool,
                    std::numeric_limits<KeyType>::max(), NULL));
}

template <typename Key, typename Hasher, typename Traits>
void TypedIndexer<Key, Hasher, Traits>::sealIndex()
{
    typename PostingTableType::Iterator it = m_pPostingTable->iterator();
    while (it.hasNext())
    {
        typename PostingTableType::KeyValueType& kv = it.next();
        kv.second->seal();
    }
}

template <typename Key, typename Hasher, typename Traits>
void TypedIndexer<Key, Hasher, Traits>::addField(const FX_NS(document)::AnalyzedField* pField)
{
    const FX_NS(document)::AnalyzedField::TokenView* pTokenView =
        pField->getTokenView();
    if (pTokenView)
    {
        loc_t lastPos = 0;
        typename FX_NS(analyzer)::TokenView::Iterator it =
            pTokenView->iterator();
        while (it.hasNext())
        {
            const FX_NS(analyzer)::Token& token = it.next();
            this->addToken(token, lastPos);
        }
    }
    else
    {
        loc_t lastPos = 0;
        const FX_NS(document)::Field* pRawField = pField->getField();
        if (!pRawField)
        {
            this->addToken(NULL, 0, lastPos);
        }
        else
        {
            this->addToken(pRawField->getValue().c_str(), 
                           pRawField->getValue().length(), lastPos);
        }
    }
}

template <typename Key, typename Hasher, typename Traits>
void TypedIndexer<Key, Hasher, Traits>::commitDocument(docid_t docId)
{
    FX_TRACE("Commit document: [%d]", docId);
    
    for (typename PostingVector::const_iterator it =
             m_modifiedPostings.begin(); 
         it != m_modifiedPostings.end(); ++it)
    {
        (*it)->commitDocument(docId);
    }
    m_modifiedPostings.clear();
}

template <typename Key, typename Hasher, typename Traits>
void TypedIndexer<Key, Hasher, Traits>::commit(FX_NS(store)::FileSystemPtr& pFileSys,
                         const std::string& sSuffix)
{
    std::string sFieldName = getFieldSchema()->getName();

    FX_NS(store)::OutputStreamPtr pDictStream = pFileSys->createFile(
            BarrelDirectory::getFilePath(sFieldName, 
                    DICT_FILEEXT, sSuffix));

    TypedIndexCommitter<PostingWriterType, WITH_POSITION> committer;
    committer.init(pFileSys, sFieldName, sSuffix);

    FX_TRACE("Commit to: %s.dic.%s", sFieldName.c_str(), sSuffix.c_str());

    size_t nSize = m_pPostingTable->size();
    FX_TRACE("Commit: total terms: %u", nSize);
    
    DictType dict;
    if (nSize > 0)
    {
        //TODO: optimize
        dict.reserve(nSize);
        typename PostingTableType::Iterator it = m_pPostingTable->iterator();
        while (it.hasNext())
        {
            typename PostingTableType::KeyValueType& kv = it.next();
            offset_t startOff = committer.commit(kv.second);
            //FX_TRACE("Commit term: term: %llu, offset: %llu", kv.first, startOff);
            dict.pushBack(kv.first, startOff);
        }
    }
    dict.save(pDictStream); 
}

template <typename Key, typename Hasher, typename Traits>
void TypedIndexer<Key, Hasher, Traits>::commitMeta(FieldMeta& fieldMeta)
{
    fieldMeta.totalTermCount = m_nTotalTerms;
    fieldMeta.distinctTermCount = m_pPostingTable->size();
}

template <typename Key, typename Hasher, typename Traits>
void TypedIndexer<Key, Hasher, Traits>::clear()
{
    if (m_pPostingTable)
    {
        typename PostingTableType::Iterator iter = m_pPostingTable->iterator();
        while (iter.hasNext())
        {
            iter.next().second->~PostingWriterType();
        }
        m_pPostingTable->clear();
        m_pPostingTable.reset();

        m_nTotalTerms = 0;
        m_modifiedPostings.clear();
    }
}

template <typename Key, typename Hasher, typename Traits>
uint64_t TypedIndexer<Key, Hasher, Traits>::distinctNumTerms() const
{
    return (uint64_t)m_pPostingTable->size();
}

template <typename Key, typename Hasher, typename Traits>
uint64_t TypedIndexer<Key, Hasher, Traits>::totalTerms() const
{
    return m_nTotalTerms;
}

template <typename Key, typename Hasher, typename Traits>
inline void TypedIndexer<Key, Hasher, Traits>::addToken(
        const FX_NS(analyzer)::Token& token, loc_t& lastPos)
{
    KeyType key = (KeyType)0;
    if (token.getTextLength() > 0)
    {
        key = m_hasher(token.getTextValue(), token.getTextLength());
        lastPos += token.getPosIncrement();
        doAddToken(key, lastPos);
    }
    else
    {
        ++lastPos;
    }
}

template <typename Key, typename Hasher, typename Traits>
inline void TypedIndexer<Key, Hasher, Traits>::addToken(
        const char* token, size_t len, loc_t& lastPos)
{
    KeyType key = (KeyType)0;
    if (token && len > 0)
    {
        key = m_hasher(token, len);
        doAddToken(key, lastPos);
    }
    ++lastPos;
}

template <typename Key, typename Hasher, typename Traits>
inline void TypedIndexer<Key, Hasher, Traits>::doAddToken(
        KeyType key, loc_t lastPos)
{
    PostingWriterType* pPosting = m_pPostingTable->find(key);
    if (pPosting == m_pPostingTable->emptyValue())
    {
        void* p = m_pPostingPool->postingPool->allocate(sizeof(PostingWriterType));
        pPosting = new(p) PostingWriterType();
        pPosting->init(m_pPostingPool.get());

        m_pPostingTable->insert(make_pair(key, pPosting));
        m_modifiedPostings.push_back(pPosting);
    }
    else if (pPosting->getCurrentTf() == 0) 
    {
        m_modifiedPostings.push_back(pPosting);
    }
    pPosting->addPosition(lastPos);
    ++m_nTotalTerms;
}

FX_NS_END

#endif //__FX_TYPEDINDEXER_H

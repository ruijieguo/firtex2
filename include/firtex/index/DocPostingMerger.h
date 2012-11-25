//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-08-22 23:11:25

#ifndef __FX_DOCPOSTINGMERGER_H
#define __FX_DOCPOSTINGMERGER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/DocPostingDecoder.h"
#include "firtex/index/PostingMerger.h"
#include "firtex/index/DocPostingWriter.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/utility/LooseBoundedPool.h"
#include "firtex/utility/SyncSegregatedAllocator.h"

FX_NS_DEF(index);

class DocPostingMergerStreammer
{
public:
    typedef FX_NS(utility)::LooseBoundedPool PoolType;
    typedef FX_NS(utility)::LooseBoundedPoolPtr PoolTypePtr;

    const static size_t DEFAULT_WRITE_BUFFER = 10 * 1024 * 1024; //10MB

public:
    DocPostingMergerStreammer()
        : m_nStartOff(0)
        , m_pDocListWriter(NULL)
        , m_pDocSkipListWriter(NULL)
    {
    }

    ~DocPostingMergerStreammer()
    {
        if (m_pDocSkipListWriter)
        {
            delete m_pDocSkipListWriter;
            m_pDocSkipListWriter = NULL;
        }
    }

public:
    void init(const FX_NS(store)::OutputStreamPtr& pDocStream);
    void beginMerge();
    bool isEmpty() const;
    void addDocRecord(docid_t* docBuffer, tf_t* tfBuffer, uint32_t size, bool bSeal);

    void addDocSkip(docid_t docId, ctf_t cumSumOfTf);
    void sealSkipList();
    void commitDocList(FX_NS(store)::OutputStreamPtr& pDocStream);
    void commitDocSkipList(FX_NS(store)::OutputStreamPtr& pDocStream);

    uint64_t committedBytesOfDocList() const;

protected:
    offset_t m_nStartOff;
    OutputStreamPtr m_pDocListWriter;
    DocSkipListWriter* m_pDocSkipListWriter;

    PoolTypePtr m_pPool;
    
    friend class DocPostingMerger;
    friend class PosPostingMerger;

private:
    DECLARE_STREAM_LOGGER();
};

class DocPostingMerger : public PostingMerger,
                         public TypedDocPostingWriter<DocPostingMergerStreammer>
{
public:
    DocPostingMerger();
    ~DocPostingMerger();

public:
    /**
     * Initialize output streams
     */
    void init(const FX_NS(store)::OutputStreamPtr& pDocStream);

    /**
     * Will be invoked before merging postings
     */
    void beginMerge(const MergingTermVector& terms);

    /**
     * Merge one term 
     */
    void doMerge(const MergingTerm* pTerm);

    /**
     * Will be invoked after merging
     */
    offset_t endMerge();

    /**
     * Commit posting list to output stream
     * @param pDocStream output stream of file system
     * @return start offset of posting descriptor
     */
    offset_t commit(FX_NS(store)::OutputStreamPtr& pDocStream);

protected:
    /**
     * Commit a batch of doc/tfs
     * @param baseDocId base doc id of the committed docs
     * @param docBuffer delta encoded doc record
     * @param tfBuffer tf record
     * @param nSize number of docs in buffer
     */
    void commitDocuments(docid_t baseDocId, docid_t* docBuffer,
                         tf_t* tfBuffer, uint32_t nSize);

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(DocPostingMerger);

///////////////////////////////////////////////////////////////////
//
inline void DocPostingMergerStreammer::init(const FX_NS(store)::OutputStreamPtr& pDocStream)
{
    m_pDocListWriter = pDocStream;
    if (m_pPool.isNotNull())
    {
        m_pPool->clear();
    }

    m_nStartOff = 0;

    if (m_pDocSkipListWriter)
    {
        delete m_pDocSkipListWriter;
        m_pDocSkipListWriter = NULL;
    }
}

inline void DocPostingMergerStreammer::beginMerge()
{
    m_nStartOff = m_pDocListWriter->tell();
    if (m_pDocSkipListWriter)
    {
        delete m_pDocSkipListWriter;
        m_pDocSkipListWriter = NULL;
    }
}

inline bool DocPostingMergerStreammer::isEmpty() const
{
    return (!m_pDocListWriter || m_pDocListWriter->tell() == 0);
}

inline void DocPostingMergerStreammer::addDocSkip(docid_t docId, ctf_t cumSumOfTf)
{
    if (!m_pDocSkipListWriter)
    {
        if (m_pPool.isNull())
        {
            PoolType* pPool = new PoolType(DEFAULT_WRITE_BUFFER);
            m_pPool.assign(pPool);
        }
        m_pDocSkipListWriter = new DocSkipListWriter(
                GLOBAL_CONF().Advance.Posting.maxLevel,
                m_pPool.get(), m_pPool.get());
    }
    FX_TRACE("Add doc skip: docId: %d, cumSumOfTf: %lld, off: %llu", 
                    docId, cumSumOfTf, m_pDocListWriter->tell());
    m_pDocSkipListWriter->addSkipPoint(docId, cumSumOfTf,
            m_pDocListWriter->tell() - m_nStartOff);
}

inline void DocPostingMergerStreammer::addDocRecord(docid_t* docBuffer, 
        tf_t* tfBuffer, uint32_t size, bool bSeal)
{
    const PostingCoding* pDocCoding = PostingCodingFactory::instance()->getDocCoding();
    pDocCoding->encodeInt32(*m_pDocListWriter, (const int32_t*)docBuffer, size, bSeal);
    const PostingCoding* pTfCoding = PostingCodingFactory::instance()->getTfCoding();
    pTfCoding->encodeInt32(*m_pDocListWriter, (const int32_t*)tfBuffer, size, bSeal);
}

inline void DocPostingMergerStreammer::sealSkipList()
{
    if (m_pDocSkipListWriter)
    {
        m_pDocSkipListWriter->seal();
    }
}

inline void DocPostingMergerStreammer::commitDocList(FX_NS(store)::OutputStreamPtr& pDocStream)
{
    //Nonthing to do
}

inline void DocPostingMergerStreammer::commitDocSkipList(FX_NS(store)::OutputStreamPtr& pDocStream)
{
    FIRTEX_ASSERT2(pDocStream == m_pDocListWriter);
    FX_TRACE("Commit doc skip list: off: %lld", m_pDocListWriter->tell());
    m_pDocSkipListWriter->commit(m_pDocListWriter);
}

inline uint64_t DocPostingMergerStreammer::committedBytesOfDocList() const
{
    return (uint64_t)(m_pDocListWriter->tell() - m_nStartOff);
}

inline void DocPostingMerger::init(const OutputStreamPtr& pDocStream)
{
    TypedDocPostingWriter<DocPostingMergerStreammer>::reset();

    m_streammer.init(pDocStream);
    m_curTermFreq = 0;
    m_lastDocId = 0;
    m_nUpTo = 0;
}

FX_NS_END

#endif //__FX_DOCPOSTINGMERGER_H

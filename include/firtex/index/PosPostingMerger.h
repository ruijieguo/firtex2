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
// Created	: 2010-08-26 22:08:46

#ifndef __FX_POSPOSTINGMERGER_H
#define __FX_POSPOSTINGMERGER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/DocPostingMerger.h"
#include "firtex/index/TypedPosPostingWriter.h"
#include "firtex/index/PosSkipListWriter.h"
#include <assert.h>

FX_NS_DEF(index);

class PosPostingMergerStreammer : public DocPostingMergerStreammer
{
public:
    PosPostingMergerStreammer()
        : m_nPosStartOff(0)
        , m_pPosSkipListWriter(NULL)
    {
    }

    ~PosPostingMergerStreammer()
    {
        if (m_pPosSkipListWriter)
        {
            delete m_pPosSkipListWriter;
            m_pPosSkipListWriter = NULL;
        }
    }

public:
    void init(const FX_NS(store)::OutputStreamPtr& pDocStream, 
              const FX_NS(store)::OutputStreamPtr& pPosStream);
    void beginMerge();
    void addPosRecord(loc_t* posBuffer, uint32_t size, bool bSeal);
    void addPosSkip();
    void sealSkipList();
    void commitPosList(FX_NS(store)::OutputStreamPtr& pPosStream);
    void commitPosSkipList(FX_NS(store)::OutputStreamPtr& pPosStream);

    uint64_t committedBytesOfPosList() const;

protected:
    offset_t m_nPosStartOff;
    FX_NS(store)::OutputStreamPtr m_pPosListWriter;
    PosSkipListWriter* m_pPosSkipListWriter;

    friend class PosPostingMerger;
};

class PosPostingMerger : public PostingMerger,
                         public TypedPosPostingWriter<PosPostingMergerStreammer>
{
public:
    PosPostingMerger();
    ~PosPostingMerger();

public:
    /**
     * Initialize output streams
     */
    void init(const FX_NS(store)::OutputStreamPtr& pDocStream,
              const FX_NS(store)::OutputStreamPtr& pPosStream);

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
     * Commit doc/pos list to output stream
     * @param pOutputStream output stream of file system
     * @return start offset of posting descriptor
     */
    offset_t commit(FX_NS(store)::OutputStreamPtr& pDocStream,
                 FX_NS(store)::OutputStreamPtr& pPosStream);

protected:
    void addPositions(loc_t* posBuffer, uint32_t nSize);

    void doMergeDocList(const MergingTerm* pTerm);

    void commitDocuments(docid_t baseDocId, docid_t* docBuffer,
                         tf_t* tfBuffer, uint32_t nSize);

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(PosPostingMerger);

///////////////////////////////////////////////////////////
//
inline void PosPostingMergerStreammer::init(const FX_NS(store)::OutputStreamPtr& pDocStream, 
        const FX_NS(store)::OutputStreamPtr& pPosStream)
{
    DocPostingMergerStreammer::init(pDocStream);

    m_pPosListWriter = pPosStream;

    m_nPosStartOff = 0;

    if (m_pPosSkipListWriter)
    {
        delete m_pPosSkipListWriter;
        m_pPosSkipListWriter = NULL;
    }

    if (m_pPool)
    {
        m_pPool->clear();
    }
}

inline void PosPostingMergerStreammer::beginMerge()
{
    DocPostingMergerStreammer::beginMerge();
    m_nPosStartOff = m_pPosListWriter->tell();
    if (m_pPosSkipListWriter)
    {
        delete m_pPosSkipListWriter;
        m_pPosSkipListWriter = NULL;
    }
}

inline void PosPostingMergerStreammer::addPosRecord(loc_t* posBuffer, uint32_t size, bool bSeal)
{
    const PostingCoding* pPosCoding = PostingCodingFactory::instance()->getPosCoding();
    pPosCoding->encodeInt32(*m_pPosListWriter, (const int32_t*)posBuffer, size, bSeal);
}

inline void PosPostingMergerStreammer::addPosSkip()
{
    if (!m_pPosSkipListWriter)
    {
        if (!m_pPool)
        {
            PoolType* pPool = new PoolType(DEFAULT_WRITE_BUFFER);
            m_pPool.reset(pPool);
        }
        m_pPosSkipListWriter = new PosSkipListWriter(GLOBAL_CONF().Advance.Posting.maxLevel,
                m_pPool.get(), m_pPool.get());
    }
    m_pPosSkipListWriter->addSkipPoint(m_pPosListWriter->tell() - m_nPosStartOff);
}

inline void PosPostingMergerStreammer::sealSkipList()
{
    DocPostingMergerStreammer::sealSkipList();
    if (m_pPosSkipListWriter)
    {
        m_pPosSkipListWriter->seal();
    }
}

inline void PosPostingMergerStreammer::commitPosList(FX_NS(store)::OutputStreamPtr& pPosStream)
{
    //Nonthing to do
}

inline void PosPostingMergerStreammer::commitPosSkipList(FX_NS(store)::OutputStreamPtr& pPosStream)
{
    FIRTEX_ASSERT2(pPosStream == m_pPosListWriter);
    m_pPosSkipListWriter->commit(m_pPosListWriter);
}

inline uint64_t PosPostingMergerStreammer::committedBytesOfPosList() const
{
    return (uint64_t)(m_pPosListWriter->tell() - m_nPosStartOff);
}

/////////////////////////////////////////////////////
//
inline void PosPostingMerger::init(const OutputStreamPtr& pDocStream,
                                   const OutputStreamPtr& pPosStream)
{
    TypedPosPostingWriter<PosPostingMergerStreammer>::reset();

    m_streammer.init(pDocStream, pPosStream);
}

inline void PosPostingMerger::addPositions(loc_t* posBuffer, uint32_t nSize)
{
    for (uint32_t i = 0; i < nSize; ++i)
    {
        if (m_nPosUpTo == RECORD_SIZE)
        {
            flushPosBuffer(false);
        }
        m_pPosBuffer[m_nPosUpTo++] = posBuffer[i];
    }

    if (nSize == 1)
    {
        // Remember the last position in the doc.
        // It is necessary for the terms only have one position
        m_lastPosInDoc = m_pPosBuffer[0];
    }
}

FX_NS_END

#endif //__FX_POSPOSTINGMERGER_H

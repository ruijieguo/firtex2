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
// Created	: 2010-08-23 22:42:09

#ifndef __FX_TYPEDDOCPOSTINGWRITER_H
#define __FX_TYPEDDOCPOSTINGWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/PostingPool.h"

FX_NS_DEF(index);


template <typename Streammer>
class TypedDocPostingWriter : public PostingWriter
{
public:
    typedef Streammer StreammerType;

public:
    TypedDocPostingWriter();
    ~TypedDocPostingWriter();

public:
    /**
     * Initialize output streams
     *
     * @param pPostingPool memory pool for posting-list
     */
    virtual void init(PostingPool* pPostingPool);

    ///Reset to initial state
    inline void reset();

    /**
     * Add a position of this term
     */
    void addPosition(loc_t pos);

    /**
     * Commit current building document. 
     * A document is searchable after committed
     * @param docId doc id of committing doc 
     */
    void commitDocument(docid_t docId);

    /** 
     * Whether the posting list is empty or not 
     * @return true if memory has been allocated and not empty, otherwise false.
     */
    bool isEmpty() const;

    /**
     * Seal the posting-list and release memory of buffer.
     * After sealing, modification is forbidden.
     */
    void seal();

    /**
     * Commit posting list to output stream
     * @param pDocStream output stream of file system
     * @return start offset of posting descriptor
     */
    offset_t commit(FX_NS(store)::OutputStreamPtr& pDocStream);

    /** 
     * Create decoder of this posting
     * @return decoder for visiting this posting
     */
    PostingDecoderPtr createDecoder() const;

    /**
     * Return tf of current uncommitted doc
     */
    tf_t getCurrentTf() const {return m_curTermFreq;}

protected:
    void writeMeta(FX_NS(store)::OutputStreamPtr& pDocStream);

protected:
    inline void flushBuffer(bool bSeal);

protected:
    StreammerType m_streammer;

    PostingPool* m_pPostingPool;

    docid_t* m_pDocBuffer;
    tf_t* m_pTfBuffer;
    tf_t m_curTermFreq;
    docid_t m_lastDocId;
    uint32_t m_nUpTo;
    
private:
    DECLARE_STREAM_LOGGER();
};

/////////////////////////////////////////////////
//
SETUP_STREAM_LOGGER_TEMP(index, TypedDocPostingWriter);

template <typename Streammer>
TypedDocPostingWriter<Streammer>::TypedDocPostingWriter()
    : m_pPostingPool(NULL)
    , m_pDocBuffer(NULL)
    , m_pTfBuffer(NULL)
    , m_curTermFreq(0)
    , m_lastDocId(0)
    , m_nUpTo(0)
{

}

template <typename Streammer>
TypedDocPostingWriter<Streammer>::~TypedDocPostingWriter() 
{
}

template <typename Streammer>
void TypedDocPostingWriter<Streammer>::init(PostingPool* pPostingPool)
{
    m_pPostingPool = pPostingPool;
    // m_pDocBuffer = (docid_t*)pPostingPool->bufferPool->allocate(RECORD_SIZE * sizeof(docid_t));
    // m_pTfBuffer = (tf_t*)pPostingPool->bufferPool->allocate(RECORD_SIZE * sizeof(tf_t));
}

template <typename Streammer>
inline void TypedDocPostingWriter<Streammer>::reset()
{
    m_curTermFreq = 0;
    m_lastDocId = 0;
    m_nUpTo = 0;
}

template <typename Streammer>
inline void TypedDocPostingWriter<Streammer>::addPosition(loc_t pos)
{
    ++m_curTermFreq;
}

template <typename Streammer>
inline void TypedDocPostingWriter<Streammer>::flushBuffer(bool bSeal)
{
    FX_TRACE("Flush: first docId: %d, tf: %d, length: %d",
             m_pDocBuffer[0], m_pTfBuffer[0], m_nUpTo);

    m_streammer.addDocRecord(m_pDocBuffer, m_pTfBuffer, m_nUpTo, bSeal);
    if (m_nUpTo == RECORD_SIZE)
    {
        m_streammer.addDocSkip(m_lastDocId, getTermMeta().getCTF());
    }

    m_nUpTo = 0;
}

template <typename Streammer>
void TypedDocPostingWriter<Streammer>::commitDocument(docid_t docId)
{
    if (m_nUpTo == RECORD_SIZE)
    {
        flushBuffer(false);
    }

    TermMeta& termMeta = getTermMeta();
    df_t& docFreq = termMeta.getDocFreq();

    /// the first document in this posting
    if (docFreq > 0)
    {
        if (docFreq == 1)
        {
            m_pDocBuffer = (docid_t*)m_pPostingPool->bufferPool->allocate(RECORD_SIZE * sizeof(docid_t));
            m_pTfBuffer = (tf_t*)m_pPostingPool->bufferPool->allocate(RECORD_SIZE * sizeof(tf_t));
            m_pDocBuffer[0] = m_lastDocId;
            m_pTfBuffer[0] = (tf_t)termMeta.getCTF();
        }

        m_pDocBuffer[m_nUpTo] = docId - m_lastDocId;
        m_pTfBuffer[m_nUpTo] = m_curTermFreq;
    }

    ++docFreq;
    termMeta.getCTF() += m_curTermFreq;

    FX_TRACE("Commit docId: %d, previous doc: [%d], tf: %d", docId, 
             m_lastDocId, m_curTermFreq);

    m_curTermFreq = 0;
    m_lastDocId = docId;
    ++m_nUpTo;
}

template <typename Streammer>
bool TypedDocPostingWriter<Streammer>::isEmpty() const
{
    return (m_nUpTo == 0 && m_streammer.isEmpty());
}

template <typename Streammer>
void TypedDocPostingWriter<Streammer>::seal()
{
    if (m_nUpTo > 0) 
    {
        if ((getTermMeta().getDocFreq() > 1) || (m_nUpTo > 1))
        {
            flushBuffer(true);
        }
        else 
        {
            //only one document 
            m_nUpTo = 0;
        }
        m_streammer.sealSkipList();
    }
}

template <typename Streammer>
offset_t TypedDocPostingWriter<Streammer>::commit(FX_NS(store)::OutputStreamPtr& pDocStream)
{
    offset_t off0 = pDocStream->tell();
    off0 = off0;
    if (m_nUpTo > 0)
    {
        seal();
    }

    if (getTermMeta().getDocFreq() > 1)
    {
        m_streammer.commitDocList(pDocStream);
    }

    offset_t startOff = pDocStream->tell();
    FX_TRACE("commit: befor commit off: %lld", off0);
    FX_TRACE("commit: startOff: %lld", startOff);

    writeMeta(pDocStream);
    if (getTermMeta().getDocFreq() >= (df_t)RECORD_SIZE)
    {
        m_streammer.commitDocSkipList(pDocStream);
    }
    m_lastDocId = 0;
    m_nUpTo = 0;

    return startOff;
}

template <typename Streammer>
void TypedDocPostingWriter<Streammer>::writeMeta(FX_NS(store)::OutputStreamPtr& pDocStream)
{
    const TermMeta& termMeta = getTermMeta();
    df_t docFreq = termMeta.getDocFreq();
    ctf_t ctf = termMeta.getCTF();

    uint64_t docLen = m_streammer.committedBytesOfDocList();
    FX_TRACE("write meta: length: %llu, df: %d, ctf: %lld", docLen, docFreq, ctf);
    pDocStream->writeVInt64(docFreq);
    if (docFreq == 1)
    {
        if (ctf > 1)
        {
            pDocStream->writeVInt32((m_lastDocId << 1) + 1);
            pDocStream->writeVInt64(ctf);
        }
        else 
        {
            pDocStream->writeVInt32(m_lastDocId << 1);
        }
    }
    else 
    {
        pDocStream->writeVInt64((int64_t)docLen);
        pDocStream->writeVInt32(m_lastDocId);
        pDocStream->writeVInt64(ctf);
    }
}

template <typename Streammer>
PostingDecoderPtr TypedDocPostingWriter<Streammer>::createDecoder() const
{
    //TODO:
    return PostingDecoderPtr();
}


FX_NS_END

#endif //__FX_TYPEDDOCPOSTINGWRITER_H

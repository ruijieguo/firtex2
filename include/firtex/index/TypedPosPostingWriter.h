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
// Created	: 2010-08-26 22:58:21

#ifndef __FX_TYPEDPOSPOSTINGWRITER_H
#define __FX_TYPEDPOSPOSTINGWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TypedDocPostingWriter.h"

FX_NS_DEF(index);

template <typename Streammer>
class TypedPosPostingWriter : public TypedDocPostingWriter<Streammer>
{
public:
    typedef Streammer StreammerType;

    using TypedDocPostingWriter<Streammer>::m_pPostingPool;
    using TypedDocPostingWriter<Streammer>::m_curTermFreq;
    using TypedDocPostingWriter<Streammer>::m_nUpTo;
    using TypedDocPostingWriter<Streammer>::m_lastDocId;
    using TypedDocPostingWriter<Streammer>::m_streammer;
    
public:
    TypedPosPostingWriter();
    ~TypedPosPostingWriter();

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
     * A document is searchable when committed
     * @param docId doc id of committing doc 
     */
    void commitDocument(docid_t docId);

    /**
     * Seal the position-list and release memory of buffers.
     * After sealing, modification is forbidden.
     */
    void seal();

    /**
     * Commit doc/pos list to output stream
     * @param pOutputStream output stream of file system
     * @return start offset of posting descriptor
     */
    virtual offset_t commit(FX_NS(store)::OutputStreamPtr& pDocStream,
                            FX_NS(store)::OutputStreamPtr& pPosStream);

    /**
     * Commit posting list to output stream. This method is 
     * unsupported.
     * @param pOutputStream output stream of file system
     * @return start offset of posting descriptor
     */
    offset_t commit(FX_NS(store)::OutputStreamPtr& pOutputStream);

    /** 
     * Create decoder of this posting
     * @return decoder for visiting this posting
     */
    PostingDecoderPtr createDecoder() const;

protected:
    void writeMeta(FX_NS(store)::OutputStreamPtr& pDocStream,
                   offset_t posStartOff);

    inline void flushPosBuffer(bool bSeal);

protected:
    loc_t* m_pPosBuffer;
    uint32_t m_nPosUpTo;
    loc_t m_lastPosInDoc;

private:
    DECLARE_STREAM_LOGGER();
};

/////////////////////////////////////////
//
SETUP_STREAM_LOGGER_TEMP(index, TypedPosPostingWriter);

template <typename Streammer>
TypedPosPostingWriter<Streammer>::TypedPosPostingWriter() 
    : m_pPosBuffer(NULL)
    , m_nPosUpTo(0)
    , m_lastPosInDoc(0)
{
}

template <typename Streammer>
TypedPosPostingWriter<Streammer>::~TypedPosPostingWriter() 
{
}

template <typename Streammer>
void TypedPosPostingWriter<Streammer>::init(PostingPool* pPostingPool)
{
    TypedDocPostingWriter<Streammer>::init(pPostingPool);
//    m_pPosBuffer = (loc_t*)pPostingPool->bufferPool->allocate(RECORD_SIZE * sizeof(loc_t));
}

template <typename Streammer>
inline void TypedPosPostingWriter<Streammer>::reset()
{
    TypedDocPostingWriter<Streammer>::reset();

    m_nPosUpTo = 0;
    m_lastPosInDoc = 0;
}

template <typename Streammer>
inline void TypedPosPostingWriter<Streammer>::addPosition(loc_t pos)
{
    if (m_nPosUpTo == RECORD_SIZE)
    {
        this->flushPosBuffer(false);
    }

    ctf_t ctf = this->getTermMeta().getCTF() + m_curTermFreq;
    if (ctf > 0)
    {
        if (ctf == 1)
        {
            m_pPosBuffer = (loc_t*)m_pPostingPool->bufferPool->allocate(RECORD_SIZE * sizeof(loc_t));
            m_pPosBuffer[0] = m_lastPosInDoc;
            
            if (m_curTermFreq == 0)
            {
                //The first document only has one position
                m_lastPosInDoc = 0;
            }
        }
        m_pPosBuffer[m_nPosUpTo] = pos - m_lastPosInDoc;
    }
    m_lastPosInDoc = pos;
    ++m_curTermFreq;
    ++m_nPosUpTo;
}

template <typename Streammer>
inline void TypedPosPostingWriter<Streammer>::flushPosBuffer(bool bSeal)
{
    FX_TRACE("Flush pos buffer: first pos: %d, length: %u",
                    m_pPosBuffer[0], m_nPosUpTo);

    m_streammer.addPosRecord(m_pPosBuffer, m_nPosUpTo, bSeal);
    if (m_nPosUpTo == RECORD_SIZE)
    {
        m_streammer.addPosSkip();
    }
    m_nPosUpTo = 0;
}

template <typename Streammer>
void TypedPosPostingWriter<Streammer>::commitDocument(docid_t docId)
{
    FX_TRACE("Commit doc: %d", docId);
    TypedDocPostingWriter<Streammer>::commitDocument(docId);
    
    /// If the first document only has one position, m_lastPosInDoc will be not reset
    if (this->getTermMeta().getCTF() > 1)
    {
        m_lastPosInDoc = 0;
    }
}

template <typename Streammer>
void TypedPosPostingWriter<Streammer>::seal()
{
    const TermMeta& termMeta = this->getTermMeta();
    if (m_nPosUpTo > 0)
    {
        if ((termMeta.getCTF() > 1) || (m_nPosUpTo > 1))
        {
            flushPosBuffer(true);
        }
        else
        {
            m_nPosUpTo = 0;
        }
    }

    TypedDocPostingWriter<Streammer>::seal();
}

template <typename Streammer>
offset_t TypedPosPostingWriter<Streammer>::commit(
        FX_NS(store)::OutputStreamPtr& pDocStream, 
        FX_NS(store)::OutputStreamPtr& pPosStream)
{
    if (m_nPosUpTo > 0)
    {
        seal();
    }

    const TermMeta& termMeta = this->getTermMeta();
    if (termMeta.getCTF() > 1)
    {
        // Commit pos-list
        m_streammer.commitPosList(pPosStream);
    }

    offset_t posEndOff = pPosStream->tell();
    if (termMeta.getCTF() > 1)
    {
        int64_t posLen = (int64_t)m_streammer.committedBytesOfPosList();
        FX_TRACE("Commit: pos length: %lld", posLen);
        pPosStream->writeVInt64(posLen);
    }

    if (termMeta.getCTF() >= RECORD_SIZE)
    {
        m_streammer.commitPosSkipList(pPosStream);
    }

    if (termMeta.getDocFreq() > 1)
    {
        m_streammer.commitDocList(pDocStream);
    }

    offset_t startOff = pDocStream->tell();

    writeMeta(pDocStream, posEndOff);
    if (termMeta.getDocFreq() >= RECORD_SIZE)
    {
        m_streammer.commitDocSkipList(pDocStream);
    }

    m_lastDocId = 0;

    FX_TRACE("Commit pos posting: %lld", startOff);
    m_nPosUpTo = 0;
    m_nUpTo = 0;

    return startOff;
}

template <typename Streammer>
offset_t TypedPosPostingWriter<Streammer>::commit(
        FX_NS(store)::OutputStreamPtr& pOutputStream)
{
    FIRTEX_THROW(UnsupportedOpException, "Unsupported operation.");
}

template <typename Streammer>
void TypedPosPostingWriter<Streammer>::writeMeta(
        FX_NS(store)::OutputStreamPtr& pDocStream, offset_t posStartOff)
{
    const TermMeta& termMeta = this->getTermMeta();
    df_t docFreq = termMeta.getDocFreq();
    ctf_t ctf = termMeta.getCTF();

    uint64_t docLen = m_streammer.committedBytesOfDocList();

    pDocStream->writeVInt64(docFreq);
    if (docFreq == 1)
    {
        if (ctf > 1)
        {
            pDocStream->writeVInt32((m_lastDocId << 1) + 1);
            pDocStream->writeVInt64(ctf);
            pDocStream->writeVInt64(posStartOff);
        }
        else 
        {
            pDocStream->writeVInt32(m_lastDocId << 1);
            pDocStream->writeVInt64(m_lastPosInDoc);
        }
    }
    else 
    {
        pDocStream->writeVInt64((int64_t)docLen);
        pDocStream->writeVInt32(m_lastDocId);
        pDocStream->writeVInt64(ctf);
        pDocStream->writeVInt64(posStartOff);
    }
    FX_TRACE("Write meta: length: %llu, df: %d, ctf: %lld, pos offset: %lld, lastDocId: %d",
                    docLen, docFreq, ctf, posStartOff, m_lastDocId);
}

template <typename Streammer>
PostingDecoderPtr TypedPosPostingWriter<Streammer>::createDecoder() const
{
    //TODO:
    return PostingDecoderPtr();
}

FX_NS_END

#endif //__FX_TYPEDPOSPOSTINGWRITER_H

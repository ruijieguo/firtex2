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
// Created	: 2010-08-12 22:49:02

#ifndef __FX_POSPOSTINGWRITER_H
#define __FX_POSPOSTINGWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/DocPostingWriter.h"
#include "firtex/index/PosSkipListWriter.h"
#include "firtex/index/TypedPosPostingWriter.h"

FX_NS_DEF(index);

class PosPostingStreammer : public DocPostingStreammer
{
public:
    PosPostingStreammer();
    ~PosPostingStreammer();

public:
    void init(PostingPool* pPostingPool);
    
    void addPosRecord(loc_t* posBuffer, uint32_t size, bool bSeal);
    void addPosSkip();
    void sealSkipList();
    void commitPosList(FX_NS(store)::OutputStreamPtr& pPosStream);
    void commitPosSkipList(FX_NS(store)::OutputStreamPtr& pPosStream);

    inline uint64_t committedBytesOfPosList() const;

protected:
    ByteSliceOutputStream* m_pPosListWriter;
    PosSkipListWriter* m_pPosSkipListWriter;

    friend class PosPostingWriter;

private:
    DECLARE_STREAM_LOGGER();
};

class PosPostingWriter : public TypedPosPostingWriter<PosPostingStreammer>
{
public:
    PosPostingWriter() {}
    ~PosPostingWriter() {}

public:
    /**
     * Initialize output streams
     *
     * @param pPostingPool memory pool for posting-list
     */
    virtual void init(PostingPool* pPostingPool);

    /** 
     * Create decoder of this posting
     * @return decoder for visiting this posting
     */
    PostingDecoderPtr createDecoder() const;
};

DEFINE_TYPED_PTR(PosPostingWriter);

/////////////////////////////////////////
//inline functions
inline PosPostingStreammer::PosPostingStreammer() 
    : m_pPosListWriter(NULL)
    , m_pPosSkipListWriter(NULL)
{
}

inline PosPostingStreammer::~PosPostingStreammer()
{
    if (m_pPosListWriter)
    {
        delete m_pPosListWriter;
        m_pPosListWriter = NULL;
    }

    if (m_pPosSkipListWriter)
    {
        delete m_pPosSkipListWriter;
        m_pPosSkipListWriter = NULL;
    }
}

inline void PosPostingStreammer::init(PostingPool* pPostingPool)
{
    DocPostingStreammer::init(pPostingPool);
}

inline void PosPostingStreammer::addPosRecord(loc_t* posBuffer, uint32_t size, bool bSeal)
{
    if (!m_pPosListWriter)
    {
        m_pPosListWriter = new ByteSliceOutputStream(m_pPostingPool->postingPool);
    }
    
    const PostingCoding* pPosCoding = PostingCodingFactory::instance()->getPosCoding();
    pPosCoding->encodeInt32(*m_pPosListWriter, (const int32_t*)posBuffer, size, bSeal);
}

inline void PosPostingStreammer::addPosSkip()
{
    if (!m_pPosSkipListWriter)
    {
        m_pPosSkipListWriter = new PosSkipListWriter(
                GLOBAL_CONF().Advance.Posting.maxLevel,
                m_pPostingPool->postingPool, m_pPostingPool->bufferPool);
    }
    FX_TRACE("Add pos skip: %lld", m_pPosListWriter->tell());
    m_pPosSkipListWriter->addSkipPoint(m_pPosListWriter->tell());
}

inline void PosPostingStreammer::sealSkipList()
{
    DocPostingStreammer::sealSkipList();
    if (m_pPosSkipListWriter)
    {
        m_pPosSkipListWriter->seal();
    }
}

inline void PosPostingStreammer::commitPosList(FX_NS(store)::OutputStreamPtr& pPosStream)
{
    if (m_pPosListWriter)
    {
        m_pPosListWriter->copyTo(*pPosStream);
    }
}

inline void PosPostingStreammer::commitPosSkipList(FX_NS(store)::OutputStreamPtr& pPosStream)
{
    m_pPosSkipListWriter->commit(pPosStream);
}

inline uint64_t PosPostingStreammer::committedBytesOfPosList() const
{
    return (m_pPosListWriter ? (uint64_t)m_pPosListWriter->tell() : 0);
}

FX_NS_END

#endif //__FX_POSPOSTINGWRITER_H

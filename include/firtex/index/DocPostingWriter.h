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
// Created	: 2010-08-09 22:44:09

#ifndef __FX_DOCPOSTINGWRITER_H
#define __FX_DOCPOSTINGWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/PostingWriter.h"
#include "firtex/index/DocSkipListWriter.h"
#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/index/TypedDocPostingWriter.h"
#include "firtex/index/PostingPool.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/Pool.h"
#include "firtex/index/PostingCodingFactory.h"

FX_NS_DEF(index);

class DocPostingStreammer
{
public:
    DocPostingStreammer();
    ~DocPostingStreammer();

public:
    void init(PostingPool* pPostingPool);
    bool isEmpty() const;
    void addDocRecord(docid_t* docBuffer, tf_t* tfBuffer, uint32_t size, bool bSeal);

    void addDocSkip(docid_t docId, ctf_t cumSumOfTf);

    void sealSkipList();
    void commitDocList(FX_NS(store)::OutputStreamPtr& pOutputStream);
    void commitDocSkipList(FX_NS(store)::OutputStreamPtr& pOutputStream);

    uint64_t committedBytesOfDocList() const;

protected:
    PostingPool* m_pPostingPool;
    ByteSliceOutputStream* m_pDocListWriter;
    DocSkipListWriter* m_pDocSkipListWriter;

    friend class DocPostingWriter;
    friend class PosPostingWriter;

private:
    DECLARE_STREAM_LOGGER();
};

class DocPostingWriter : public TypedDocPostingWriter<DocPostingStreammer>
{
public:
    DocPostingWriter() {}
    ~DocPostingWriter() {}

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

DEFINE_TYPED_PTR(DocPostingWriter);

/////////////////////////////////////////////////
//
inline DocPostingStreammer::DocPostingStreammer()
    : m_pPostingPool(NULL)
    , m_pDocListWriter(NULL)
    , m_pDocSkipListWriter(NULL)
{
}

inline DocPostingStreammer::~DocPostingStreammer()
{
    if (m_pDocListWriter)
    {
        delete m_pDocListWriter;
        m_pDocListWriter = NULL;
    }

    if (m_pDocSkipListWriter)
    {
        delete m_pDocSkipListWriter;
        m_pDocSkipListWriter = NULL;
    }

    m_pPostingPool = NULL;
}

inline void DocPostingStreammer::init(PostingPool* pPostingPool)
{
    m_pPostingPool = pPostingPool;
}

inline bool DocPostingStreammer::isEmpty() const
{
    return (!m_pDocListWriter || m_pDocListWriter->tell() == 0);
}

inline void DocPostingStreammer::addDocSkip(docid_t docId, ctf_t cumSumOfTf)
{
    if (!m_pDocSkipListWriter)
    {
        m_pDocSkipListWriter = new DocSkipListWriter(
                GLOBAL_CONF().Advance.Posting.maxLevel,
                m_pPostingPool->postingPool, m_pPostingPool->bufferPool);
    }
    FX_TRACE("Add doc skip: %d, cumSumOfTf: %lld, off: %lld", 
                    docId, cumSumOfTf, m_pDocListWriter->tell());
    m_pDocSkipListWriter->addSkipPoint(docId, cumSumOfTf,
            m_pDocListWriter->tell());
}

inline void DocPostingStreammer::addDocRecord(docid_t* docBuffer, 
        tf_t* tfBuffer, uint32_t size, bool bSeal)
{
    if (!m_pDocListWriter)
    {
        m_pDocListWriter = new ByteSliceOutputStream(m_pPostingPool->postingPool);
    }

    const PostingCoding* pDocCoding = PostingCodingFactory::instance()->getDocCoding();
    pDocCoding->encodeInt32(*m_pDocListWriter, (const int32_t*)docBuffer, size, bSeal);

    const PostingCoding* pTfCoding = PostingCodingFactory::instance()->getTfCoding();
    pTfCoding->encodeInt32(*m_pDocListWriter, tfBuffer, size, bSeal);
}

inline void DocPostingStreammer::sealSkipList()
{
    if (m_pDocSkipListWriter)
    {
        m_pDocSkipListWriter->seal();
    }
}

inline void DocPostingStreammer::commitDocList(FX_NS(store)::OutputStreamPtr& pOutputStream)
{
    if (m_pDocListWriter)
    {
        m_pDocListWriter->copyTo(*(pOutputStream.get()));
    }
}

inline void DocPostingStreammer::commitDocSkipList(FX_NS(store)::OutputStreamPtr& pOutputStream)
{
    m_pDocSkipListWriter->commit(pOutputStream);
}

inline uint64_t DocPostingStreammer::committedBytesOfDocList() const
{
    return (m_pDocListWriter ? (uint64_t)m_pDocListWriter->tell() : 0);
}

FX_NS_END

#endif //__FX_DOCPOSTINGWRITER_H

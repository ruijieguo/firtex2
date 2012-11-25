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
// Created	: 2010-08-09 22:32:00

#ifndef __FX_POSTINGWRITER_H
#define __FX_POSTINGWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermMeta.h"
#include "firtex/index/PostingDecoder.h"
#include "firtex/store/OutputStream.h"

FX_NS_DEF(index);

class PostingWriter
{
public:
    PostingWriter() {}
    virtual ~PostingWriter() {}

public:
    /**
     * Return term meta of this posting
     */
    const TermMeta& getTermMeta() const { return m_termMeta;}
    TermMeta& getTermMeta() { return m_termMeta;}

public:
    /**
     * Commit current building document. 
     * A document is searchable when committed
     * @param docId doc id of committing doc 
     */
    virtual void commitDocument(docid_t docId) = 0;

    /** 
     * Whether the posting list is empty or not 
     * @return true if memory has been allocated and not empty, otherwise false.
     */
    virtual bool isEmpty() const = 0;

    /**
     * Seal the posting-list and release memory of buffer.
     * After sealing, modification is forbidden.
     */
    virtual void seal() = 0;

    /**
     * Commit posting list to output stream
     * @param pOutputStream output stream of file system
     * @return start offset of posting descriptor
     */
    virtual offset_t commit(FX_NS(store)::OutputStreamPtr& pOutputStream) = 0;

    /** 
     * Create decoder of this posting
     * @return decoder for visiting this posting
     */
    virtual PostingDecoderPtr createDecoder() const = 0;

protected:
    TermMeta m_termMeta;
};

DEFINE_TYPED_PTR(PostingWriter);

FX_NS_END

#endif //__FX_POSTINGWRITER_H

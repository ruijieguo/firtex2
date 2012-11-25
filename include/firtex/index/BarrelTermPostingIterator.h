//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2005/12/26
//

#ifndef __BARRELTERMPOSTINGITERATOR_H
#define __BARRELTERMPOSTINGITERATOR_H

#include "firtex/store/InputStream.h"
#include "firtex/utility/BitVector.h"
#include "firtex/index/TermPostingIterator.h"
#include "firtex/index/TermInfo.h"
#include "firtex/index/PostingDecoder.h"

FX_NS_DEF(index);

class BarrelTermPostingIterator : public TermPostingIterator
{
public:
    static const size_t	DEFAULT_BUFFER_SIZE = 128;

public:
    BarrelTermPostingIterator();
    virtual ~BarrelTermPostingIterator(void);

public:
    /**
     * Reset iterator
     */
    virtual void init(const PostingDecoderPtr& pPosting,
                      const FX_NS(utility)::BitVector* pDocFilter);

    /**
     * Return term meta
     */
    const TermMeta& getTermMeta() const;

    /**
     * Move to the next documents block
     * @param pDocs documents block
     * @param pFreqs term frequency block
     * @return number of documents in the block
     */			
    df_t nextDocs(docid_t*& pDocs, tf_t*& pFreqs);

    /** 
     * skip to document whose id is equal to or
     *  greater than <i>docId</i>
     */ 
    docid_t skipTo(docid_t docId);

    /**
     * Get current doc id. Only valid after 
     * calling {@link skipTo()} 
     * @return current doc id
     */
    docid_t doc() const;
	
    /**
     * get current in document tf.only valid after calling {@link next()} or 
     * {@link skipTo(docid_t target,docid_t& nearTarget)} and returns true
     * @return current in document tf
     */
    tf_t freq();

    /**
     * Return posting decoder
     */
    PostingDecoderPtr getPostingDecoder() {return m_pPostingDecoder;}

protected:
    /**
     * Read and decode doc-list
     * @return false if no doc to decode
     */
    bool decode(docid_t docId);

protected:
    PostingDecoderPtr m_pPostingDecoder;
    const FX_NS(utility)::BitVector* m_pDocFilter;

    docid_t m_docBuffer[DEFAULT_BUFFER_SIZE];
    tf_t m_tfBuffer[DEFAULT_BUFFER_SIZE];

    uint32_t m_nDocCountInBuffer;
    uint32_t m_nVisitDocInBuffer;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif


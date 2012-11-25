//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2005/12/20
//
#ifndef __TERMPOSTINGITERATOR_H
#define __TERMPOSTINGITERATOR_H

#include "Term.h"
#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermMeta.h"
#include "firtex/index/PostingDecoder.h"

FX_NS_DEF(index)
		
class TermPostingIterator
{
public:
    TermPostingIterator(void) {}
    virtual ~TermPostingIterator(void) {}

public:
    /**
     * Get term meta
     */
    virtual const TermMeta& getTermMeta() const = 0;

    /**
     * Move to the next documents block
     * @param docs documents block
     * @param freqs term frequency block
     * @return number of documents in the block
     */			
    virtual df_t nextDocs(docid_t*& docs, tf_t*& freqs) = 0;

    /** 
     * skip to document whose id is equal to or
     *  greater than <i>docId</i>
     */ 
    virtual docid_t skipTo(docid_t docId) = 0;
	
    /**
     * Get current doc id. Only valid after 
     * calling {@link skipTo()} 
     * @return current doc id
     */
    virtual docid_t doc() const = 0;

    /**
     * Get current in document tf. Only valid after 
     * calling {@link skipTo()} 
     * @return current in document tf
     */
    virtual tf_t freq() = 0;

    /**
     * Move to next position block
     * @param positions position block
     * @return size of position block
     */
    virtual tf_t nextPositions(loc_t*& positions) { return 0;}

    /**	 
     * Skip to position which is equal to or 
     * greater than <i>pos</i>
     */ 
    virtual loc_t skipToPosition(loc_t pos) { return INVALID_POS;};

    /**
     * Return posting decoder
     */
    virtual PostingDecoderPtr getPostingDecoder() {return PostingDecoderPtr();}
};

DEFINE_TYPED_PTR(TermPostingIterator);

FX_NS_END

#endif

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
// Created : 2011-07-15 20:01:12

#ifndef __FX_INMEMKEYWORDTERMREADER_H
#define __FX_INMEMKEYWORDTERMREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/keyword/KeywordIndexer.h"
#include "firtex/index/common/TypedInMemTermReader.h"

FX_NS_DEF(index);

class InMemKeywordTermReader : public TypedInMemTermReader<uint64_t, IndexKeyHasher<uint64_t>, DocPostingTraits<uint64_t> >
{
public:
    typedef DocPostingTraits<uint64_t>::PostingTableType PostingTableType;
    typedef DocPostingTraits<uint64_t>::PostingTableTypePtr PostingTableTypePtr;

public:
    InMemKeywordTermReader() {}
    InMemKeywordTermReader(const InMemKeywordTermReader& src)
        : TypedInMemTermReader<uint64_t, IndexKeyHasher<uint64_t>, DocPostingTraits<uint64_t> >(src) {}
    ~InMemKeywordTermReader() {}

public:
    /**
     * Clone a new object
     */
    virtual TermReader* clone() const;

protected:
    /**
     * Create term iterator
     *
     * @param pPostingTable posting table
     * @param pLowerTerm lower bound term, inclusive
     * @param pUpperTerm upper bound term, inclusive
     * 
     * @return term iterator instance
     */
    virtual TermIteratorPtr createTermIterator(
            const PostingTableTypePtr& pPostingTable, 
            const Term* pLowerTerm,
            const Term* pUpperTerm) const;

    /**
     * Create term posting iterator
     * 
     * @param pPostingDecoder posting decoder
     * @return Term posting iterator instance
     */
    virtual TermPostingIteratorPtr createTermPostingIterator(
            const PostingDecoderPtr& pPostingDecoder) const;

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_INMEMKEYWORDTERMREADER_H

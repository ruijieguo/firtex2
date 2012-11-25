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
// Created : 2012-04-22 10:39:04

#ifndef __FX_INMEMSIMPLENUMERICTERMREADER_H
#define __FX_INMEMSIMPLENUMERICTERMREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/DocPostingWriter.h"
#include "firtex/index/common/TypedInMemTermReader.h"
#include "firtex/index/numeric/InMemSimpleNumericTermIterator.h"

FX_NS_DEF(index);

template <typename T>
class InMemSimpleNumericTermReader : public TypedInMemTermReader<T, NumericHasher<T>, DocPostingTraits<T> >
{
public:
    typedef T KeyType;
    typedef typename DocPostingTraits<KeyType>::PostingTableType PostingTableType;
    typedef typename DocPostingTraits<KeyType>::PostingTableTypePtr PostingTableTypePtr;

public:
    InMemSimpleNumericTermReader() {}
    InMemSimpleNumericTermReader(const InMemSimpleNumericTermReader& src)
        : TypedInMemTermReader<KeyType, NumericHasher<KeyType>, DocPostingTraits<KeyType> >(src) {}

    ~InMemSimpleNumericTermReader() {}

public:
    /**
     * Clone a new object
     */
    virtual TermReader* clone() const
    {
        return new InMemSimpleNumericTermReader(*this);
    }

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
            const Term* pUpperTerm) const
    {
        InMemSimpleNumericTermIterator<KeyType>* pTmpIter =
            new InMemSimpleNumericTermIterator<KeyType>(
                    this->getFieldSchema(), this->getDocFilter());
        TermIteratorPtr pTermIter(pTmpIter);
        if (!pTmpIter->init(pPostingTable, pLowerTerm, pUpperTerm))
        {
            return TermIteratorPtr();
        }
        return pTermIter;
    }


    /**
     * Create term posting iterator
     * 
     * @param pPostingDecoder posting decoder
     * @return Term posting iterator instance
     */
    virtual TermPostingIteratorPtr createTermPostingIterator(
            const PostingDecoderPtr& pDecoder) const
    {
        BarrelTermPostingIterator* pIt = new BarrelTermPostingIterator();
        TermPostingIteratorPtr pItPtr(pIt);
        pIt->init(pDecoder, this->getDocFilter());
        return pItPtr;
    }
};

FX_NS_END

#endif //__FX_INMEMSIMPLENUMERICTERMREADER_H

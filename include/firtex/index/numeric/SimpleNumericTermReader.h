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
// Created : 2012-04-21 17:38:13

#ifndef __FX_SIMPLENUMERICTERMREADER_H
#define __FX_SIMPLENUMERICTERMREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStreamPool.h"
#include "firtex/index/common/TypedTermReader.h"
#include "firtex/index/numeric/SimpleNumericTermIterator.h"
#include "firtex/index/numeric/NumericHasher.h"

FX_NS_DEF(index);

template <typename T>
class SimpleNumericTermReader : public TypedTermReader<T, NumericHasher<T> >
{
public:
    typedef T KeyType;

public:
    SimpleNumericTermReader() {}
    ~SimpleNumericTermReader() {}

    /**
     * Create term iterator
     *
     * @param pLowerTerm lower bound term, inclusive
     * @param pUpperTerm upper bound term, inclusive
     * 
     * @return Term iterator instance
     */
    virtual TermIteratorPtr createTermIterator(
            const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
            const Term* pLowerTerm, const Term* pUpperTerm) const;

    /**
     * Create term posting iterator
     * 
     * @param pStreamPool stream pool
     * @param nTermOffset term offset in doc-list file
     *
     * @return Term posting iterator instance
     */
    virtual TermPostingIteratorPtr createTermPostingIterator(
            const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
            offset_t nTermOffset) const;

public:
    /**
     * Clone a new object
     */
    TermReader* clone() const;

private:
    DECLARE_LOGGER();
};

///////////////////////////////////////////////////////////////
//
template <typename T>
TermIteratorPtr SimpleNumericTermReader<T>::createTermIterator(
        const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
        const Term* pLowerTerm, const Term* pUpperTerm) const
{
    InputStreamPtr pDocStream = pStreamPool->getInputStream(
            this->getFileName(DOC_POSTING_FILEEXT));
    if (pDocStream.isNull())
    {
        FIRTEX_THROW(FileIOException, "Open posting-list file of [%s] FAILED.",
                     this->getFieldSchema()->getName().c_str());
    }

    SimpleNumericTermIterator<KeyType>* pTmpIter =
        new SimpleNumericTermIterator<KeyType>(
            this->getFieldSchema(), this->getDocFilter());
    TermIteratorPtr pTermIter(pTmpIter);
    if (!pTmpIter->init(this->getDict(), pDocStream, pLowerTerm,
                        pUpperTerm, pStreamPool.get()))
    {
        return TermIteratorPtr();
    }
    return pTermIter;
}

template <typename T>
TermPostingIteratorPtr SimpleNumericTermReader<T>::createTermPostingIterator(
        const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
        offset_t nTermOffset) const
{
    InputStreamPtr pDocStream = pStreamPool->getInputStream(
                this->getFileName(DOC_POSTING_FILEEXT));
    if (pDocStream.isNull())
    {
        FIRTEX_THROW(FileIOException, _T("Open posting-list file of [%s] FAILED."),
                     this->getFieldSchema()->getName().c_str());
    }
    
    pDocStream->seek(nTermOffset);
    DocPostingDecoder* pDecoder = new DocPostingDecoder();
    PostingDecoderPtr pDecoderPtr(pDecoder);
    pDecoder->init(pDocStream, NULL, pStreamPool.get());

    BarrelTermPostingIterator* pTmpIter = new BarrelTermPostingIterator();
    TermPostingIteratorPtr pIter(pTmpIter);
    pTmpIter->init(pDecoderPtr, this->getDocFilter());
    return pIter;
}

template <typename T>
TermReader* SimpleNumericTermReader<T>::clone() const
{
    return new SimpleNumericTermReader<T>(*this);
}


FX_NS_END

#endif //__FX_SIMPLENUMERICTERMREADER_H

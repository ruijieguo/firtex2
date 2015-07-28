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
// Created : 2011-12-25 17:06:06

#ifndef __FX_TYPEDTERMITERATOR_H
#define __FX_TYPEDTERMITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermIterator.h"
#include "firtex/index/DictLoader.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/index/BarrelTermPositionIterator.h"
#include "firtex/document/FieldSchema.h"
#include "firtex/store/InputStreamPool.h"

FX_NS_DEF(index);

template <typename Key, typename Traits>
class TypedTermIterator : public TermIterator
{
public:
    typedef Key KeyType;
    typedef Traits TraitsType;
    typedef typename Traits::TermType TermType;
    typedef typename Traits::DictType DictType;
    typedef typename Traits::PostingDecoderType PostingDecoderType;
    typedef typename Traits::PostingIteratorType PostingIteratorType;

    DEFINE_TYPED_PTR(PostingDecoderType);

public:
    TypedTermIterator(const FX_NS(document)::FieldSchema* pFieldSchema,
                     const FX_NS(utility)::BitVector* pDocFilter);
    ~TypedTermIterator();

public:
    /**
     * Inialize
     */
    bool init(const DictType* pDict,
              const FX_NS(store)::InputStreamPtr& pDocStream,
              const FX_NS(store)::InputStreamPtr& pPosStream,
              const Term* pLowerTerm, const Term* pUpperTerm,
              const FX_NS(store)::InputStreamPool* pInStreamPool);

    /**
     * Check if the iterator has next element or not
     */
    bool hasNext();
    
    /**
     * Return the next element
     * Example code:
     * 
     *   while (hasNext())
     *   {
     *      TermEntry t = next();
     *   }
     */
    TermEntry next();

    /**
     * Return number of terms
     */
    int64_t size() const;

protected:
    const FX_NS(document)::FieldSchema* m_pFieldSchema;
    const FX_NS(utility)::BitVector* m_pDocFilter;
    const DictType* m_pDict;
    typename DictType::Iterator m_iterator;

    TermType m_curTerm;
    PostingDecoderTypePtr m_pCurDecoder;
    BarrelTermPostingIterator* m_pCurIterator;
    TermPostingIteratorPtr m_pIteratorPtr;

    const FX_NS(store)::InputStreamPool* m_pInStreamPool;
    FX_NS(store)::InputStreamPtr m_pDocStream;
    FX_NS(store)::InputStreamPtr m_pPosStream;

private:
    DECLARE_LOGGER();
};

////////////////////////////////////////////////
///
SETUP_LOGGER_TEMP2(index, TypedTermIterator);

template <typename Key, typename Traits>
TypedTermIterator<Key, Traits>::TypedTermIterator(const FX_NS(document)::FieldSchema* pFieldSchema,
                                     const FX_NS(utility)::BitVector* pDocFilter)
    : m_pFieldSchema(pFieldSchema)
    , m_pDocFilter(pDocFilter)
    , m_pDict(NULL)
    , m_pCurDecoder(NULL)
    , m_pCurIterator(NULL)
    , m_pInStreamPool(NULL)
{
}

template <typename Key, typename Traits>
TypedTermIterator<Key, Traits>::~TypedTermIterator() 
{
    if (m_pInStreamPool)
    {
        if (m_pDocStream)
        {
            m_pInStreamPool->releaseInputStream(m_pDocStream);
        }
        if (m_pPosStream)
        {
            m_pInStreamPool->releaseInputStream(m_pPosStream);
        }
    }
}

template <typename Key, typename Traits>
bool TypedTermIterator<Key, Traits>::init(const DictType* pDict,
                             const FX_NS(store)::InputStreamPtr& pDocStream,
                             const FX_NS(store)::InputStreamPtr& pPosStream,
                             const Term* pLowerTerm,
                             const Term* pUpperTerm, 
                             const FX_NS(store)::InputStreamPool* pStreamPool)
{
    const TermType* pLTerm = NULL;
    if (pLowerTerm)
    {
        pLTerm = dynamic_cast<const TermType*>(pLowerTerm);
        if (!pLTerm)
        {
            FIRTEX_THROW(CastException, "Cast term type FAILED.");
        }
    }
    
    const TermType* pUTerm = NULL;
    if (pUpperTerm)
    {
        pUTerm = dynamic_cast<const TermType*>(pUpperTerm);
        if (!pUTerm)
        {
            FIRTEX_THROW(CastException, "Cast term type FAILED.");
        }
    }
    
    if (pLTerm && pUTerm && pUTerm->compare(pLTerm) <= 0)
    {
        return false;
    }

    m_pDict = pDict;
    if (pLTerm || pUTerm)
    {
        KeyType startKey = pLTerm ? pLTerm->getValue() : 0;
        KeyType endKey = pUTerm ? pUTerm->getValue() :
                         KeyType(std::numeric_limits<KeyType>::max());
        m_iterator = m_pDict->rangeIterator(startKey, endKey);
        if (!m_iterator.hasNext())
        {
            return false;
        }
    }
    else 
    {
        m_iterator = m_pDict->iterator();
    }
    m_pInStreamPool = pStreamPool;
    m_pDocStream = pDocStream;
    m_pPosStream = pPosStream;
 
    m_pCurDecoder.reset(new PostingDecoderType());
    m_pCurIterator = new PostingIteratorType();
    m_pIteratorPtr.reset(m_pCurIterator);
    return true;
}

template <typename Key, typename Traits>
bool TypedTermIterator<Key, Traits>::hasNext()
{
    return m_iterator.hasNext();
}

template <typename Key, typename Traits>
TermIterator::TermEntry TypedTermIterator<Key, Traits>::next()
{
    typename DictType::KeyValueType kv = m_iterator.next();
    m_curTerm.setValue(kv.first);
    
    TermIterator::TermEntry entry;
    entry.term = &m_curTerm;
    // FX_TRACE("Next term: term: %llu, offset: %llu",
    //          m_curTerm.getValue(), kv.second);
    m_pDocStream->seek(kv.second);

    m_pCurDecoder->init(m_pDocStream, m_pPosStream, NULL);
    m_pCurIterator->init(m_pCurDecoder, m_pDocFilter);
    entry.postingIterator = m_pIteratorPtr;

    return entry;
}

template <typename Key, typename Traits>
int64_t TypedTermIterator<Key, Traits>::size() const
{
    return (int64_t)m_iterator.size();
}

FX_NS_END

#endif //__FX_TYPEDTERMITERATOR_H

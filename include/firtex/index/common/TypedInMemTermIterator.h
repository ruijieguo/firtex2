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
// Created : 2011-12-25 18:34:23

#ifndef __FX_TYPEDINMEMTERMITERATOR_H
#define __FX_TYPEDINMEMTERMITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

template <typename Key, typename Traits>
class TypedInMemTermIterator : public TermIterator
{
public:
    typedef Key KeyType;
    typedef Traits TraitsType;
    typedef typename Traits::TermType TermType;
    typedef typename Traits::InMemDictType InMemDictType;
    typedef typename Traits::PostingDecoderType PostingDecoderType;
    typedef typename Traits::PostingDecoderTypePtr PostingDecoderTypePtr;
    typedef typename Traits::PostingIteratorType PostingIteratorType;
    typedef typename Traits::PostingTableType PostingTableType;
    typedef typename Traits::PostingTableTypePtr PostingTableTypePtr;
    
public:
    TypedInMemTermIterator(const FX_NS(document)::FieldSchema* pFieldSchema,
                           const FX_NS(utility)::BitVector* pDocFilter);
    virtual ~TypedInMemTermIterator();

public:
    /**
     * Initialize
     *
     * @param pPostingTable in-memory posting table
     * @param pLowerTerm lower term
     * @param pUpperTerm Upper term
     * @return false if failure
     * @throw throw CastException if given a wrong term type
     */
    bool init(const PostingTableTypePtr& pPostingTable,
              const Term* pLowerTerm, const Term* pUpperTerm);

    /**
     * Check if the iterator has next element or not
     */
    virtual bool hasNext();
    
    /**
     * Return the next element
     * Example code:
     * 
     *   while (hasNext())
     *   {
     *      TermEntry t = next();
     *   }
     */
    virtual TermEntry next();

    /**
     * Return number of terms
     */
    virtual int64_t size() const;

private:
    const FX_NS(document)::FieldSchema* m_pFieldSchema;
    const FX_NS(utility)::BitVector* m_pDocFilter;

    InMemDictType m_dict;
    typename InMemDictType::Iterator m_iterator;

    TermType m_curTerm;
    PostingDecoderPtr m_pCurDecoder;
    BarrelTermPostingIterator* m_pCurIterator;
    TermPostingIteratorPtr m_pIteratorPtr;

private:
    DECLARE_LOGGER();
};

////////////////////////////////////////////
SETUP_LOGGER_TEMP2(index, TypedInMemTermIterator);

template <typename Key, typename Traits>
TypedInMemTermIterator<Key, Traits>::TypedInMemTermIterator(
        const FX_NS(document)::FieldSchema* pFieldSchema,
        const FX_NS(utility)::BitVector* pDocFilter) 
    : m_pFieldSchema(pFieldSchema)
    , m_pDocFilter(pDocFilter)
    , m_pCurIterator(NULL)
{
}
template <typename Key, typename Traits>
TypedInMemTermIterator<Key, Traits>::~TypedInMemTermIterator() 
{
}
template <typename Key, typename Traits>
bool TypedInMemTermIterator<Key, Traits>::init(
        const PostingTableTypePtr& pPostingTable,
        const Term* pLowerTerm, const Term* pUpperTerm)
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

    size_t nSize = pPostingTable->size();
    if (nSize == 0)
    {
        return false;
    }

    m_dict.reserve(nSize);
    typename PostingTableType::Iterator it = pPostingTable->iterator();
    while (it.hasNext())
    {
        typename PostingTableType::KeyValueType& kv = it.next();
        m_dict.pushBack(kv.first, kv.second);
    }
    m_dict.finalize();

    if (pLTerm || pUTerm)
    {
        KeyType startKey = pLTerm ? pLTerm->getValue() : 0;
        KeyType endKey = pUTerm ? pUTerm->getValue() :
                         KeyType(std::numeric_limits<KeyType>::max());
        m_iterator = m_dict.rangeIterator(startKey, endKey);
        if (!m_iterator.hasNext())
        {
            return false;
        }
    }
    else 
    {
        m_iterator = m_dict.iterator();
    }

    m_pCurDecoder.reset(new PostingDecoderType());
    m_pCurIterator = new PostingIteratorType();
    m_pIteratorPtr.reset(m_pCurIterator);
    return true;
}

template <typename Key, typename Traits>
bool TypedInMemTermIterator<Key, Traits>::hasNext()
{
    return m_iterator.hasNext();
}

template <typename Key, typename Traits>
TermIterator::TermEntry TypedInMemTermIterator<Key, Traits>::next()
{
    typename InMemDictType::KeyValueType kv = m_iterator.next();
    m_curTerm.setValue(kv.first);
    
    TermIterator::TermEntry entry;
    entry.term = &m_curTerm;

    m_pCurDecoder = kv.second->createDecoder();
    m_pCurIterator->init(m_pCurDecoder, m_pDocFilter);
    entry.postingIterator = m_pIteratorPtr;

    return entry;
}
template <typename Key, typename Traits>
int64_t TypedInMemTermIterator<Key, Traits>::size() const
{
    return (int64_t)m_iterator.size();
}

FX_NS_END

#endif //__FX_TYPEDINMEMTERMITERATOR_H

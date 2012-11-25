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
// Created : 2011-12-25 18:48:33

#ifndef __FX_TYPEDINMEMTERMREADER_H
#define __FX_TYPEDINMEMTERMREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermReader.h"
#include "firtex/utility/HashMap.h"
#include "firtex/index/PosPostingWriter.h"
#include "firtex/index/text/TextIndexer.h"

FX_NS_DEF(index);

template <typename Key, typename Hasher, typename Traits>
class TypedInMemTermReader : public TermReader
{
public:
    typedef Key KeyType;
    typedef Hasher HasherType;

    typedef typename Traits::PostingTableType PostingTableType;
    typedef FX_NS(common)::SharedPtr<PostingTableType> PostingTableTypePtr;

public:
    TypedInMemTermReader();
    TypedInMemTermReader(const TypedInMemTermReader<Key, Hasher, Traits>& src);
    ~TypedInMemTermReader();

public:
    /**
     * Initialize
     *
     * @param pFieldSchema field schema of this term reader
     * @param pPostingTable in-memory posting table
     * @param pDocFilter deleted document filter
     */
    void init(const FX_NS(document)::FieldSchema* pFieldSchema,
              const PostingTableTypePtr& pPostingTable,
              const FX_NS(utility)::BitVector* pDocFilter);

    /**
     * Return term iterator of field
     * @param sField field name
     */
    virtual TermIteratorPtr termIterator(const std::string& sField) const;

    /**
     * Return term iterator of term range
     * @param pLowerTerm lower bound term, inclusive
     * @param pUpperTerm upper bound term, inclusive
     */
    virtual TermIteratorPtr termIterator(const Term* pLowerTerm,
                                         const Term* pUpperTerm) const;

    /**
     * Find a term in dictionary
     * @param pTerm term to find
     * @return term posting iterator of current found term
     */
    virtual TermPostingIteratorPtr seek(const Term* pTerm) const;

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
            const Term* pUpperTerm) const = 0;

    /**
     * Create term posting iterator
     * 
     * @param pPostingDecoder posting decoder
     * @return Term posting iterator instance
     */
    virtual TermPostingIteratorPtr createTermPostingIterator(
            const PostingDecoderPtr& pPostingDecoder) const = 0;

private:
    PostingTableTypePtr m_pPostingTable;
    const FX_NS(utility)::BitVector* m_pDocFilter;
    HasherType m_hasher;

private:
    DECLARE_LOGGER();
};

///////////////////////////////////////////////////
//
SETUP_LOGGER_TEMP3(index, TypedInMemTermReader);

template <typename Key, typename Hasher, typename Traits>
TypedInMemTermReader<Key, Hasher, Traits>::TypedInMemTermReader() 
    : m_pDocFilter(NULL)
{
}

template <typename Key, typename Hasher, typename Traits>
TypedInMemTermReader<Key, Hasher, Traits>::TypedInMemTermReader(
        const TypedInMemTermReader<Key, Hasher, Traits>& src) 
    : TermReader(src)
    , m_pPostingTable(src.m_pPostingTable)
    , m_pDocFilter(src.m_pDocFilter)
{
}

template <typename Key, typename Hasher, typename Traits>
TypedInMemTermReader<Key, Hasher, Traits>::~TypedInMemTermReader() 
{
}

template <typename Key, typename Hasher, typename Traits>
void TypedInMemTermReader<Key, Hasher, Traits>::init(
        const FX_NS(document)::FieldSchema* pFieldSchema,
        const PostingTableTypePtr& pPostingTable,
        const FX_NS(utility)::BitVector* pDocFilter)
{
    m_pFieldSchema = pFieldSchema;
    m_pPostingTable = pPostingTable;
    m_pDocFilter = pDocFilter;
}

template <typename Key, typename Hasher, typename Traits>
TermIteratorPtr TypedInMemTermReader<Key, Hasher, Traits>::termIterator(
        const std::string& sField) const
{
    if (FX_NS(utility)::strCompare(getFieldSchema()->getName().c_str(), 
                   sField.c_str()))
    {
        return TermIteratorPtr();
    }

    return createTermIterator(m_pPostingTable, NULL, NULL);
}

template <typename Key, typename Hasher, typename Traits>
TermIteratorPtr TypedInMemTermReader<Key, Hasher, Traits>::termIterator(
        const Term* pLowerTerm, const Term* pUpperTerm) const
{
    return createTermIterator(m_pPostingTable, pLowerTerm, pUpperTerm);
}

template <typename Key, typename Hasher, typename Traits>
TermPostingIteratorPtr TypedInMemTermReader<Key, Hasher, Traits>::seek(
        const Term* pTerm) const
{
    if (FX_NS(utility)::strCompare(getFieldSchema()->getName().c_str(), 
                                   pTerm->getField().c_str()))
    {
        return TermPostingIteratorPtr();
    }
    
    KeyType key = (KeyType)(-1);
    const std::string& strValue = pTerm->stringValue();
    if (!strValue.empty())
    {
        key = m_hasher(strValue.c_str(), strValue.length());
    }
    else 
    {
        const TypedTerm<KeyType>* pTypedTerm = pTerm->cast<KeyType>();
        if (pTypedTerm)
        {
            key = pTypedTerm->getValue();
        }
        else
        {
            return TermPostingIteratorPtr();
        }
    }

    typename PostingTableType::ValueType pPosting = m_pPostingTable->find(key);
    if (pPosting != m_pPostingTable->emptyValue())
    {
        return createTermPostingIterator(pPosting->createDecoder());
    }
    return TermPostingIteratorPtr();
}

FX_NS_END

#endif //__FX_TYPEDINMEMTERMREADER_H

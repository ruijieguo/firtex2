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
// Created : 2011-12-25 13:55:32

#ifndef __FX_TYPEDTERMREADER_H
#define __FX_TYPEDTERMREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/DictLoader.h"

FX_NS_DEF(index);

template <typename Key, typename Hasher>
class TypedTermReader : public TermReader
{
public:
    typedef Key KeyType;
    typedef Hasher HasherType;

    typedef DictLoader<KeyType> DictLoaderType;
    typedef typename DictLoaderType::DictType DictType;

public:
    TypedTermReader();
    TypedTermReader(const TypedTermReader& src);
    ~TypedTermReader();

public:
    /**
     * Open an index barrel on file system
     *
     * @param pFileSystem file system 
     * @param pStreamPool input stream pool
     * @param sBarrelSuffix barrel name of index
     * @param pFieldSchema field schema of this term reader
     * @param pDocFilter deleted document filter
     * @throw throw FileIOException or IndexCollapseException when failed.
     */
    void open(const FX_NS(store)::FileSystemPtr& pFileSystem,
              const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
              const std::string& sBarrelSuffix, 
              const FX_NS(document)::FieldSchema* pFieldSchema, 
              const FX_NS(utility)::BitVector* pDocFilter);

    /**
     * Return term iterator of field
     * @param sField field name
     */
    TermIteratorPtr termIterator(const std::string& sField) const;

    /**
     * Return term iterator of term range
     * @param pLowerTerm lower bound term, inclusive
     * @param pUpperTerm upper bound term, inclusive
     */
    TermIteratorPtr termIterator(const Term* pLowerTerm,
                                 const Term* pUpperTerm) const;

    /**
     * Find a term in dictionary
     * @param pTerm term to find
     * @return term posting iterator of current found term
     */
    TermPostingIteratorPtr seek(const Term* pTerm) const;

protected:
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
            const Term* pLowerTerm, const Term* pUpperTerm) const = 0;

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
            offset_t nTermOffset) const = 0;

protected:
    const DictType* getDict() const 
    {
        return m_pDict;
    }

private:
    const DictType* m_pDict;
    std::string m_sDictName;
    HasherType m_hasher;
    
private:
    DECLARE_LOGGER();
};

SETUP_LOGGER_TEMP2(index, TypedTermReader);

template <typename Key, typename Hasher>
TypedTermReader<Key, Hasher>::TypedTermReader() 
    : m_pDict(NULL)
{
}

template <typename Key, typename Hasher>
TypedTermReader<Key, Hasher>::TypedTermReader(const TypedTermReader& src)
    : TermReader(src)
{
    m_sDictName = src.m_sDictName;
    if (!DictLoaderType::instance()->incRefDict(m_sDictName))
    {
        m_pDict = NULL;
        FX_LOG(ERROR, "Add reference of dict: [%s] FAILE.", m_sDictName.c_str());
    }
    else 
    {
        m_pDict = src.m_pDict;
    }
}

template <typename Key, typename Hasher>
TypedTermReader<Key, Hasher>::~TypedTermReader() 
{
    if (m_pDict)
    {
        DictLoaderType::instance()->releaseDict(m_sDictName);
        m_pDict = NULL;
    }
}

template <typename Key, typename Hasher>
void TypedTermReader<Key, Hasher>::open(const FX_NS(store)::FileSystemPtr& pFileSystem,
                                        const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
                                        const std::string& sSuffix, 
                                        const FX_NS(document)::FieldSchema* pFieldSchema,
                                        const FX_NS(utility)::BitVector* pDocFilter)
{
    TermReader::open(pFileSystem, pStreamPool, sSuffix, pFieldSchema, pDocFilter);

    m_sDictName =  this->getFileName(DICT_FILEEXT);
    m_pDict = DictLoaderType::instance()->loadDict(m_sDictName, pFileSystem);
}

template <typename Key, typename Hasher>
TermIteratorPtr TypedTermReader<Key, Hasher>::termIterator(const std::string& sField) const
{
    if (FX_NS(utility)::strCompare(getFieldSchema()->getName().c_str(), 
                                   sField.c_str()))
    {
        return TermIteratorPtr();
    }

    return this->createTermIterator(m_pStreamPool, NULL, NULL);
}

template <typename Key, typename Hasher>
TermIteratorPtr TypedTermReader<Key, Hasher>::termIterator(const Term* pLowerTerm,
        const Term* pUpperTerm) const
{
    return createTermIterator(m_pStreamPool, pLowerTerm, pUpperTerm);
}

template <typename Key, typename Hasher>
TermPostingIteratorPtr TypedTermReader<Key, Hasher>::seek(const Term* pTerm) const
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

    offset_t startOffOfCurTerm = (offset_t)(-1);
   
    if (m_pDict->find(key, startOffOfCurTerm))
    {
        return createTermPostingIterator(m_pStreamPool, startOffOfCurTerm);
    }
    return TermPostingIteratorPtr();
}

FX_NS_END

#endif //__FX_TYPEDTERMREADER_H

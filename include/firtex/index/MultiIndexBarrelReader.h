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
// Created	: 2005/11/27
//
#ifndef __FX_MULTIINDEXBARRELREADER_H
#define __FX_MULTIINDEXBARRELREADER_H

#include "firtex/common/SharedPtr.h"
#include "firtex/index/IndexBarrelReader.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/SingleIndexBarrelReader.h"
#include "firtex/index/IndexBarrelWriter.h"
#include "firtex/index/Index.h"
#include <map>
#include <vector>

FX_NS_DEF(index)
	
class MultiIndexBarrelReader : public IndexBarrelReader
{
public:
    class Entry
    {
    public:
        Entry(const FX_NS(store)::FileSystemPtr& pFileSys,
              const FX_NS(document)::DocumentSchema* pDocSchema,
              const ComponentBuilder* pComponentBuilder)
            : m_pBarrelInfo(NULL)
        {
            m_pBarrel = new SingleIndexBarrelReader(pFileSys, pDocSchema,
                                                    pComponentBuilder);
        }

        Entry(const BarrelInfo* pBarrelInfo, SingleIndexBarrelReader* pIndexBarrelReader)
            : m_pBarrelInfo(pBarrelInfo)
            , m_pBarrel(pIndexBarrelReader)
        {
        }

        Entry(const Entry& src)
            : m_pBarrelInfo(src.m_pBarrelInfo)
        {
            m_pBarrel = dynamic_cast<SingleIndexBarrelReader*>(src.m_pBarrel->clone());
        }

        ~Entry()
        {
            delete m_pBarrel;
            m_pBarrel = NULL;
            m_pBarrelInfo = NULL;
        }
    public:
        void init(const BarrelInfo* pBarrelInfo,
                  const FX_NS(utility)::BitVector* pDocFilter)
        {
            m_pBarrelInfo = pBarrelInfo;
            m_pBarrel->open(pBarrelInfo, pDocFilter);
        }

        Entry* clone() const
        {
            return new Entry(*this);
        }

    public:
        const BarrelInfo* m_pBarrelInfo;
        SingleIndexBarrelReader* m_pBarrel;
    };

    DEFINE_TYPED_PTR(Entry);

    typedef std::vector<EntryPtr> BarrelVector;
    typedef std::vector<Index*> IndexVector;

public:
    class Iterator
    {
    public:
        Iterator(MultiIndexBarrelReader* pMultiReader);
        Iterator(const Iterator& _iter);
        virtual ~Iterator(void);

    public:
        /**
         * reset the iterator
         */
        void reset();

        /**
         * whether reach to end or not
         * @return true for not, false for yes
         */
        bool hasNext() const;

        /**
         * retrieval current element
         * @return current element
         */
        MultiIndexBarrelReader::EntryPtr next();

        /**
         * number of elements
         * @return number of elements
         */
        size_t size()const;

    private:
        MultiIndexBarrelReader*	m_pMultiReader;
        BarrelVector::iterator m_iterator;
    };

public:
    MultiIndexBarrelReader(const FX_NS(store)::FileSystemPtr& pFileSys,
                           const FX_NS(document)::DocumentSchema* pDocSchema,
                           const ComponentBuilder* pComponentBuilder);
    MultiIndexBarrelReader(const MultiIndexBarrelReader& src);
    virtual ~MultiIndexBarrelReader(void);

public:
    /**
     * open index barrels of multi index
     * @param indices index vector
     */
    void open(const IndexVector& indices);

    /**
     * Open index barrels
     * @param pBarrelsInfo barrels info of index
     * @param pDocFilter deleted document filter
     */
    void open(const BarrelsInfoPtr& pBarrelsInfo,
              const DeletedDocumentFilterPtr& pDocFilter);

    /**
     * Reopen the index barrel
     */
    void reopen(const BarrelsInfoPtr& pBarrelsInfo,
                const DeletedDocumentFilterPtr& pDocFilter);

public:
    /**
     * Create stored fields reader 
     * @return stored fields reader object which is thread unsafe
     */
    StoredFieldsReaderPtr createStoredFieldsReader() const;

    /**
     * Create term reader			 
     * @return term reader
     */
    TermReaderPtr termReader() const;

    /**
     * Create term reader of specific field
     * @return term reader
     */
    TermReaderPtr termReader(const tstring& sField) const;

    /**
     * Create the iterator of forward index by specified field
     * @return forward index iterator
     */
    ForwardIndexIteratorPtr forwardIndexReader(const tstring& sField) const;

    /**
     * Return length norm by field name
     * @param sField field name
     */
    LengthNormIteratorPtr lengthNorm(const tstring& sField) const;

    /**
     * Return length norm by field id
     * @param fieldId field id
     */
    LengthNormIteratorPtr lengthNorm(fieldid_t fieldId) const;

    /**
     * Return fields information
     */
    const FieldsInfo& getFieldsInfo() const;

    /**
     * Clone a new instance
     */
    IndexBarrelReader* clone() const;

public:
    /**
     * Return iterator of document fields
     * @return iterator 
     */
    MultiIndexBarrelReader::Iterator iterator();

protected:
    void addIndex(const Index* pIndex);
    
private:
    BarrelsInfoPtr m_pBarrelsInfo;
    BarrelVector m_readers;
    
    TermReaderPtr m_pTermReader;

    DeletedDocumentFilterPtr m_pDeletedDocFilter;
};

DEFINE_TYPED_PTR(MultiIndexBarrelReader);

FX_NS_END

#endif 

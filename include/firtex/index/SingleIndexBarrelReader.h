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
#ifndef __FX_SINGLEINDEXBARRELREADER_H
#define __FX_SINGLEINDEXBARRELREADER_H

#include <vector>
#include "firtex/common/SharedPtr.h"
#include "firtex/index/IndexBarrelReader.h"
#include "firtex/index/LengthNormReader.h"
#include "firtex/utility/BitVector.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/index/ForwardIndexReader.h"
#include "firtex/index/LengthNormReader.h"

FX_NS_DEF(index);

class Index;
class BarrelInfo;
class SingleIndexBarrelReader : public IndexBarrelReader
{
public:
    SingleIndexBarrelReader(const FX_NS(store)::FileSystemPtr& pFileSys,
                            const FX_NS(document)::DocumentSchema* pDocSchema,
                            const ComponentBuilder* pComponentBuilder);
    SingleIndexBarrelReader(const SingleIndexBarrelReader& src);
    virtual ~SingleIndexBarrelReader(void);

public:
    /**
     * Open an index barrel
     */
    void open(const BarrelInfo* pBarrelInfo,
              const FX_NS(utility)::BitVector* pDocFilter);

    /**
     * reopen the index barrel
     */
    void reopen(const BarrelInfo* pBarrelInfo,
                const FX_NS(utility)::BitVector* pDocFilter);

    /**
     * Close index barrel
     */
    void close();

public:

    /**
     * Create stored fields reader 
     * @return stored fields reader object which is thread unsafe
     */
    StoredFieldsReaderPtr createStoredFieldsReader() const;

    /** 
     * whether it is a deleted document or not
     * @param docId document id
     * @return true if it is deleted
     */
    bool isDeleted(docid_t docId) const;

    /** 
     * whether there is any deletions
     * @return true if there is
     */
    bool hasDeletions()const;

    /**
     * Return number of deleted documents
     * @return number of deleted documents
     */
    df_t getDeletedDocCount()const;

    /**
     * Create term reader of the index barrel
     * @return term reader
     */
    TermReaderPtr termReader() const;

    /**
     * Create term reader of the index barrel
     * @param szField field name
     * @return term reader
     */
    TermReaderPtr termReader(const std::string& sField) const;

    /**
     * Create the iterator of forward index by specified field
     * @param sField field name
     * @return forward index iterator
     */
    ForwardIndexIteratorPtr forwardIndexReader(const std::string& sField) const;

    /**
     * Return length norm by field name
     * @param sField field name
     */
    LengthNormIteratorPtr lengthNorm(const std::string& sField) const;

    /**
     * Clone a new instance
     */
    IndexBarrelReader* clone() const;

    /**
     * Return barrel info of the reader
     */
    const BarrelInfo* getBarrelInfo() const {return m_pBarrelInfo;}

public:
    /**
     * Return forward index reader by field name
     * @param sField field name
     * @return const reference of forward index reader
     */
    const ForwardIndexReader* getForwardIndexReader(const std::string& sField) const;

    /**
     * Return length norm reader
     */
    const LengthNormReader* getLengthNormReader() const;

private:
    void createTermReader(const std::string& sSuffix);
    void createForwardIndex(const std::string& sSuffix);

private:
    typedef std::vector<ForwardIndexReaderPtr> FDIndexVector;

    TermReaderPtr m_pTermReader;
    LengthNormReaderPtr m_pLengthNormReader;
    FDIndexVector m_forwardIndexReaders;

    const BarrelInfo* m_pBarrelInfo;
    const FX_NS(utility)::BitVector* m_pDocFilter;

    size_t m_bMultiIndexFields;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(SingleIndexBarrelReader);

FX_NS_END

#endif

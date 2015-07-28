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
// Created : 2011-07-16 10:30:38

#ifndef __FX_INMEMINDEXBARRELREADER_H
#define __FX_INMEMINDEXBARRELREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/IndexBarrelReader.h"
#include "firtex/index/StoredFieldsReader.h"
#include "firtex/index/LengthNormReader.h"
#include "firtex/index/ForwardIndexReader.h"

FX_NS_DEF(index);

class IndexBarrelWriter;
class IndexBarrelKeeper;
class InMemIndexBarrelReader : public IndexBarrelReader
{
public:
    InMemIndexBarrelReader(const IndexBarrelWriter* pBarrelWriter,
                           IndexBarrelKeeper* pKeeper);
    ~InMemIndexBarrelReader();

public:
    /**
     * Create stored fields reader 
     * @return stored fields reader object which is thread unsafe
     */
    virtual StoredFieldsReaderPtr createStoredFieldsReader() const;

    /**
     * Create term reader of the index barrel
     * @return term reader
     */
    virtual TermReaderPtr termReader() const;

    /**
     * Create term reader of the index barrel
     * @param szField field name
     * @return term reader
     */
    virtual TermReaderPtr termReader(const std::string& sField) const;

    /**
     * Create the iterator of forward index by specified field
     * @param sField field name
     * @return forward index iterator
     */
    virtual ForwardIndexIteratorPtr forwardIndexReader(const std::string& sField) const;

    /**
     * Create the iterator of forward index by specified field id
     * @param fieldId field id
     * @return forward index iterator
     */
    ForwardIndexIteratorPtr forwardIndexReader(fieldid_t fieldId) const;

    /**
     * Return length norm by field name
     * @param sField field name
     */
    virtual LengthNormIteratorPtr lengthNorm(const std::string& sField) const;

    /// Clone a new instance
    virtual IndexBarrelReader* clone() const;

    /// Return deleted document reader
    inline const FX_NS(utility)::BitVectorPtr getDocFilter() const;

private:
    void init(const IndexBarrelWriter* pBarrelWriter);

private:
    typedef std::vector<ForwardIndexIteratorPtr> ForwardIndexVect;

    /// Reference to index barrel keeper
    IndexBarrelKeeper* m_pKeeper;

    /// For creating In-memory stored fields reader
    std::string m_sBarrelName;

    /// In-memory index term reader
    TermReaderPtr m_pTermReader;

    /// In-memory forward index readers
    ForwardIndexVect m_forwardIndexes;    

    /// In-memory length norm reader
    LengthNormReaderPtr m_pLengthNormReader;

    /// In-memory deleted document filter
    FX_NS(utility)::BitVectorPtr m_pDocFilter;

    bool m_bHasMultiIndexFields;
    
private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(InMemIndexBarrelReader);

////////////////////////////////////////////
//
inline const FX_NS(utility)::BitVectorPtr InMemIndexBarrelReader::getDocFilter() const
{
    return m_pDocFilter;
}

FX_NS_END

#endif //__FX_INMEMINDEXBARRELREADER_H

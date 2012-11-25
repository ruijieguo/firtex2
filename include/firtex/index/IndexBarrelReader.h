//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@gmail.com
// Created	: 2005/12/27
//

#ifndef __FX_INDEXBARRELREADER_H
#define __FX_INDEXBARRELREADER_H

#include <map>
#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/StoredFieldsReader.h"
#include "firtex/index/Term.h"
#include "firtex/index/TermPostingIterator.h"
#include "firtex/index/FieldIndexer.h"
#include "firtex/index/ComponentBuilder.h"
#include "firtex/document/Document.h"
#include "firtex/document/Field.h"
#include "firtex/index/ForwardIndexIterator.h"
#include "firtex/index/LengthNormIterator.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/store/FileSystem.h"

FX_NS_DEF(index);

class IndexBarrelReader
{
public:
    IndexBarrelReader();
    IndexBarrelReader(const FX_NS(store)::FileSystemPtr& pFileSys,
                      const FX_NS(document)::DocumentSchema* pDocSchema,
                      const ComponentBuilder* pComponentBuilder);
    IndexBarrelReader(const IndexBarrelReader& src);
    virtual ~IndexBarrelReader();

public:
    /**
     * Create stored fields reader 
     * @return stored fields reader object which is thread unsafe
     */
    virtual StoredFieldsReaderPtr createStoredFieldsReader() const = 0;

    /**
     * Create term reader of the index barrel
     * @return term reader
     */
    virtual TermReaderPtr termReader() const = 0;

    /**
     * Create term reader of the index barrel
     * @param szField field name
     * @return term reader
     */
    virtual TermReaderPtr termReader(const std::string& sField) const = 0;

    /**
     * Create the iterator of forward index by specified field
     * @param sField field name
     * @return forward index iterator
     */
    virtual ForwardIndexIteratorPtr forwardIndexReader(const std::string& sField) const = 0;

    /**
     * Return length norm by field name
     * @param sField field name
     */
    virtual LengthNormIteratorPtr lengthNorm(const std::string& sField) const = 0;

    /**
     * Clone a new instance
     */
    virtual IndexBarrelReader* clone() const = 0;

public:
    FX_NS(store)::FileSystemPtr getFileSystem() const
    {
        return m_pFileSys;
    }
    const FX_NS(document)::DocumentSchema* getDocSchema() const 
    {
        return m_pDocSchema;
    }

    const ComponentBuilder* getComponentBuilder() const 
    {
        return m_pComponentBuilder;
    }

protected:
    FX_NS(store)::FileSystemPtr m_pFileSys;
    FX_NS(store)::InputStreamPoolPtr m_pInStreamPool;
    const FX_NS(document)::DocumentSchema* m_pDocSchema;
    const ComponentBuilder* m_pComponentBuilder;
};

DEFINE_TYPED_PTR(IndexBarrelReader);

FX_NS_END

#endif

//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-08-14 15:38:42

#ifndef __FX_TERMREADER_H
#define __FX_TERMREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermIterator.h"
#include "firtex/index/Term.h"
#include "firtex/index/TermPostingIterator.h"
#include "firtex/index/BarrelDirectory.h"
#include "firtex/store/FileSystem.h"
#include "firtex/document/FieldSchema.h"
#include "firtex/utility/BitVector.h"
#include "firtex/store/InputStreamPool.h"

FX_NS_DEF(index);

class TermReader
{
public:
    TermReader() : m_pFieldSchema(NULL), m_pDocFilter(NULL) {}
    TermReader(const FX_NS(document)::FieldSchema* pFieldSchema) 
        : m_pFieldSchema(pFieldSchema)
        , m_pDocFilter(NULL) 
    {}

    TermReader(const TermReader& src) 
        : m_pFileSystem(src.m_pFileSystem)
        , m_pStreamPool(src.m_pStreamPool)
        , m_pFieldSchema(src.m_pFieldSchema)
        , m_pDocFilter(src.m_pDocFilter)
        , m_sBarrelSuffix(src.m_sBarrelSuffix)
    {
    }

    virtual ~TermReader() {}

public:
    /**
     * Open an index barrel on file system
     * @param pFileSystem file system 
     * @param pStreamPool input stream pool
     * @param sBarrelSuffix barrel name of index
     * @param pFieldSchema field schema of this term reader
     * @param pDocFilter deleted document filter
     * @throw throw FileIOException or IndexCollapseException when failed.
     */
    virtual void open(const FX_NS(store)::FileSystemPtr& pFileSystem,
                      const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
                      const std::string& sBarrelSuffix, 
                      const FX_NS(document)::FieldSchema* pFieldSchema, 
                      const FX_NS(utility)::BitVector* pDocFilter)
    {
        m_pFileSystem = pFileSystem;
        m_pStreamPool = pStreamPool;
        m_sBarrelSuffix = sBarrelSuffix;
        m_pFieldSchema = pFieldSchema;
        m_pDocFilter = pDocFilter;
    }

    /**
     * Return term iterator of field
     * @param sField field name
     */
    virtual TermIteratorPtr termIterator(const std::string& sField) const = 0;

    /**
     * Return term iterator of term range
     * @param pLowerTerm lower bound term, inclusive
     * @param pUpperTerm upper bound term, inclusive
     */
    virtual TermIteratorPtr termIterator(const Term* pLowerTerm,
                                         const Term* pUpperTerm) const = 0;

    /**
     * Find a term in dictionary
     * @param pTerm term to find
     * @return term posting iterator of current found term
     */
    virtual TermPostingIteratorPtr seek(const Term* pTerm) const = 0;

    /**
     * Clone a new object
     */
    virtual TermReader* clone() const = 0;
                      
public:
    std::string getFileName(const std::string& sExt) const
    {
        return BarrelDirectory::getFilePath(m_pFieldSchema->getName(),
                sExt, m_sBarrelSuffix);
    }
    const FX_NS(document)::FieldSchema* getFieldSchema() const 
    {
        return m_pFieldSchema;
    }
    
    FX_NS(store)::FileSystemPtr getFileSystem() const 
    {
        return m_pFileSystem;
    }

    const FX_NS(utility)::BitVector* getDocFilter() const
    {
        return m_pDocFilter;
    }

protected:
    FX_NS(store)::FileSystemPtr m_pFileSystem;
    mutable FX_NS(store)::InputStreamPoolPtr m_pStreamPool;
    const FX_NS(document)::FieldSchema* m_pFieldSchema;
    const FX_NS(utility)::BitVector* m_pDocFilter;
    std::string m_sBarrelSuffix;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(TermReader);

FX_NS_END

#endif //__FX_TERMREADER_H

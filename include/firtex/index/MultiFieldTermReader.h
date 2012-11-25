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
// Created	: 2005/11/20
//
#ifndef __MULTIFIELDTERMREADER_H
#define __MULTIFIELDTERMREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/Term.h"
#include "firtex/document/DocumentSchema.h"
#include <map>

FX_NS_DEF(index)

class ComponentBuilder;
class MultiFieldTermReader : public TermReader
{
public:
    MultiFieldTermReader(const ComponentBuilder* pComBuilder);
    virtual ~MultiFieldTermReader(void);

public:
    /**
     * Open an index barrel on file system
     * @param pFileSystem file system instance
     * @param pStreamPool input stream pool
     * @param sBarrel barrel name of index
     * @param pDocSchema document schema of index
     * @param pDocFilter deleted document filter
     * @throw throw FileIOException or IndexCollapseException when failed.
     */
    void open(const FX_NS(store)::FileSystemPtr& pFileSys,
              const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
              const std::string& sBarrel,
              const FX_NS(document)::DocumentSchema* pDocSchema,
              const FX_NS(utility)::BitVector* pDocFilter);

    /**
     * Open an index barrel on file system
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

    /**
     * Clone a new object
     */
    TermReader* clone() const;

    /**
     * Return term reader of specified field
     */
    TermReaderPtr termReader(const std::string& sField) const;

public:
    /** set factory finder */
    void setComponentBuilder(const ComponentBuilder* pProducerMapper)
    {m_pComponentBuilder = pProducerMapper;}

    /**
     * add a term reader
     * @param field field name
     * @param pTermReader term reader
     */
    void addTermReader(const std::string& field, const TermReaderPtr& pTermReader);

protected:
    typedef std::map<std::string, TermReaderPtr> ReaderMap;
	
    ReaderMap m_fieldsTermReaders;
    TermReaderPtr m_pCurReader;

    const ComponentBuilder* m_pComponentBuilder;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(MultiFieldTermReader);

FX_NS_END

#endif


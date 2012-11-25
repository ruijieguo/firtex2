//
// Copyright(C) 2005--2010 FirteX Develop Team
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010/1/13
//
#ifndef __PRIMARYKEYTERMREADER_H
#define __PRIMARYKEYTERMREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/BitVector.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/primarykey/PrimaryKeyIndexer.h"
#include "firtex/index/TermInfo.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/InMemoryTermReader.h"
#include "firtex/utility/HashMap.h"
#include "firtex/utility/Hash.h"

FX_NS_DEF(index);

class PrimaryKeyIndexer;

class PrimaryKeyTermReader : public TermReader
{
public:
    typedef FX_NS(utility)::HashMap<uint64_t, docid_t> PostingTable;
    typedef FX_NS(common)::SharedPtr<PostingTable> PostingTablePtr;

public:
    PrimaryKeyTermReader();
    PrimaryKeyTermReader(const FX_NS(document)::FieldSchema* pFieldSchema, 
                         const PostingTablePtr& postingTable);
    PrimaryKeyTermReader(const PrimaryKeyTermReader& src);

    virtual ~PrimaryKeyTermReader(void);

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
    void open(const FX_NS(store)::FileSystemPtr& pFileSystem,
              const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
              const std::string& sBarrelSuffix, 
              const FX_NS(document)::FieldSchema* pFieldSchema, 
              const FX_NS(utility)::BitVector* pDocFilter);

    /**
     * Open an index barrel on file system
     * @param pFileSystem file system 
     * @param pStreamPool input stream pool
     * @param sSuffix barrel name of index
     * @param sField field name
     * @param pDocFilter deleted document filter
     * @throw throw FileIOException or IndexCollapseException when failed.
     */
    void open(const FX_NS(store)::FileSystemPtr& pFileSys,
              const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
              const std::string& sSuffix, 
              const std::string& sField,
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
     * clone the term reader
     * @return term reader, MUST be deleted by caller.
     */
    TermReader*	clone() const;

    /**
     * Get posting table 
     */
    PostingTablePtr getPostingTable() const {return m_pPostingTable;}

    docid_t lookup(const std::string& sPrimKey) const;

protected:						
    PostingTablePtr m_pPostingTable;
    
    friend class PrimaryKeyTermIterator;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(PrimaryKeyTermReader);

////////////////////////////////////////////////////////////////////
//
inline docid_t PrimaryKeyTermReader::lookup(const std::string& sPrimKey) const
{
    uint64_t key = FX_NS(utility)::Hash::hashString64(sPrimKey.c_str());
    FX_TRACE("lookup primary key: %llu", key);
    return m_pPostingTable->find(key);
}

FX_NS_END

#endif


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
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2006/1/4
//
#ifndef __FX_MULTITERMSEARCHER_H
#define __FX_MULTITERMSEARCHER_H

#include <map>
#include <vector>
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermReader.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/TermInfo.h"

FX_NS_DEF(index);

class MultiIndexBarrelReader;
class MultiTermReader :  public TermReader
{
public:
    typedef std::pair<const BarrelInfo*, TermReaderPtr> TermReaderEntry;
    typedef std::vector<TermReaderEntry> TermReaderVector;

    class Iterator
    {
    public:
        Iterator(const TermReaderVector& termReaders)
            : m_termReaders(termReaders)
        {
            m_iterator = m_termReaders.begin();
        }

        Iterator(const Iterator& _iter)
            : m_termReaders(_iter.m_termReaders)
        {
            m_iterator = m_termReaders.begin();
        }

    public:
        void reset() {m_iterator = m_termReaders.begin();}

        bool hasNext() const 
        {
            return (m_iterator != m_termReaders.end());
        }

        TermReaderEntry next() {return *m_iterator++;}

        size_t size()const
        {
            return m_termReaders.size();
        }

    private:
        
        TermReaderVector::const_iterator m_iterator;
        const TermReaderVector& m_termReaders;
    };

public:
    MultiTermReader(void);
    MultiTermReader(const MultiIndexBarrelReader* pBarrelReader);
    virtual ~MultiTermReader(void);

public:
    /**
     * Open an index barrel on file system
     * @param pFileSystem file system 
     * @param sBarrelSuffix barrel name of index
     * @param pFieldSchema field schema of this term reader
     * @param pDocFilter deleted document filter
     * @throw throw FileIOException or IndexCollapseException when failed.
     */
    void open(FX_NS(store)::FileSystemPtr& pFileSystem,
              const tstring& sBarrelSuffix, 
              const FX_NS(document)::FieldSchema* pFieldSchema, 
              const FX_NS(utility)::BitVector* pDocFilter);

    /**
     * Return term iterator of field
     * @param sField field name
     */
    TermIteratorPtr termIterator(const tstring& sField) const;

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
     * Get iterator of term readers;
     */
    Iterator iterator() const { return Iterator(m_termReaders);}

protected:
    /**
     * load reader
     */
    void loadTermReaders();

protected:
    MultiIndexBarrelReader* m_pBarrelReader;
    TermInfo m_curTermInfo;
    TermReaderVector m_termReaders;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(MultiTermReader);

FX_NS_END

#endif

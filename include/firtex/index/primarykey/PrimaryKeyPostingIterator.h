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
// Created	: 2005/12/20
//
#ifndef __PRIMARYKEYPOSTINGITERATOR_H
#define __PRIMARYKEYPOSTINGITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermPostingIterator.h"
#include "firtex/index/primarykey/PrimaryKeyPostingDecoder.h"

FX_NS_DEF(index)
		
class PrimaryKeyPostingIterator : public TermPostingIterator
{
public:

    PrimaryKeyPostingIterator(docid_t docId = INVALID_DOCID)
        : m_termMeta(1, 1)
        , m_docId(docId)
        , m_freq(1)
        , m_bEnd(false)
    {}

    virtual ~PrimaryKeyPostingIterator(void) {}

public:
    /**
     * Get term meta
     */
    const TermMeta& getTermMeta() const {return m_termMeta;}

    bool nextDoc() 
    {
        if (!m_bEnd) 
        {
            end();
            return true;
        }
        return false;
    }

    df_t nextDocs(docid_t*& docs, tf_t*& freqs)
    {
        if (!m_bEnd)
        {
            docs = &m_docId;
            freqs = &m_freq;
            end();
            return 1;
        }

        return 0;
    }

    docid_t skipTo(docid_t docId) 
    {
        if ((docId > m_docId) || m_bEnd)
        {
            return INVALID_DOCID;
        }
        return m_docId;
    }

    docid_t doc() const { return m_docId;}
    tf_t freq() {return m_freq;}

    void close() {};

    PostingDecoderPtr getPostingDecoder() 
    {
        return PostingDecoderPtr(new PrimaryKeyPostingDecoder(m_docId));
    }
public:
    void setValue(docid_t docId) 
    {
        m_docId = docId;
        m_freq = 1;
        m_bEnd = false;
    }

private:
    inline void end()
    {
        m_bEnd = true;
    }

private:
    TermMeta m_termMeta;
    docid_t m_docId;
    tf_t m_freq;
    bool m_bEnd;
};

DEFINE_TYPED_PTR(PrimaryKeyPostingIterator);

FX_NS_END

#endif

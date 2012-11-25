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
// Created	: 2010-07-11 12:07:29

#ifndef __FX_DOCSCOREFEATURE_H
#define __FX_DOCSCOREFEATURE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/TermScoreFeature.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(search);

class DocScoreFeature
{
public:
    DocScoreFeature(uint32_t nMaxTerms) 
        : m_nMaxNumTerms(nMaxTerms)
        , m_nNumTerms(0)
    {
    }

    ~DocScoreFeature() {}

public:
    /** Get number terms */
    inline size_t getNumTerms() const;

    /**
     * Return nth term score feature
     */
    inline const TermScoreFeature& getTermScoreFeature(size_t idx) const;

    /**
     * Reset to empty state
     */
    inline void reset();

    /**    
     * Fetch the next term score feature
     */
    inline TermScoreFeature& nextTermFeature();

protected:
    uint32_t m_nMaxNumTerms;
    uint32_t m_nNumTerms;
    TermScoreFeature m_termFeature[1]; //an variable array

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////////////
//
inline size_t DocScoreFeature::getNumTerms() const 
{
    return m_nNumTerms;
}

inline void DocScoreFeature::reset()
{
    m_nNumTerms = 0;
}

inline TermScoreFeature& DocScoreFeature::nextTermFeature()
{
    FIRTEX_ASSERT2(m_nMaxNumTerms >= m_nNumTerms);
    TermScoreFeature* pFeature = new (&(m_termFeature[m_nNumTerms++])) TermScoreFeature;
    return *pFeature;
}

inline const TermScoreFeature&
DocScoreFeature::getTermScoreFeature(size_t idx) const
{
    FIRTEX_ASSERT2(idx < m_nNumTerms);
    return m_termFeature[idx];
}

FX_NS_END

#endif //__FX_DOCSCOREFEATURE_H

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
// Created	: 2010-07-12 21:10:44

#ifndef __FX_TERMMETA_H
#define __FX_TERMMETA_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

class TermMeta
{
public:
    TermMeta(df_t df, ctf_t ctf) : m_docFreq(df), m_totalTermFreq(ctf)
    {
    }

    TermMeta() : m_docFreq(0), m_totalTermFreq(0)
    {
    }

    ~TermMeta() {}

public:
    /**
     * Get document frequency
     */
    inline df_t getDocFreq() const;
    inline df_t& getDocFreq();

    /**
     * Get total term frequency
     */
    inline ctf_t getCTF() const;
    inline ctf_t& getCTF();

    /** set method */
    void setDocFreq(df_t df);
    void setCTF(ctf_t ctf);

protected:
    df_t m_docFreq;
    ctf_t m_totalTermFreq;
};

typedef FX_NS(common)::SharedPtr<TermMeta> TermMetaPtr;

/////////////////////////////////////////
//
inline df_t TermMeta::getDocFreq() const
{
    return m_docFreq;
}

inline ctf_t TermMeta::getCTF() const
{
    return m_totalTermFreq;
}

inline df_t& TermMeta::getDocFreq()
{
    return m_docFreq;
}

inline ctf_t& TermMeta::getCTF()
{
    return m_totalTermFreq;
}

inline void TermMeta::setDocFreq(df_t df)
{
    m_docFreq = df;
}

inline void TermMeta::setCTF(ctf_t ctf)
{
    m_totalTermFreq = ctf;
}

FX_NS_END

#endif //__FX_TERMMETA_H

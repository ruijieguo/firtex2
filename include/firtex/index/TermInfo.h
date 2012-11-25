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
// Created	: 2005/12/23
//
#ifndef __FX_TERMINFO_H
#define __FX_TERMINFO_H

#include "firtex/common/StdHeader.h"
#include "firtex/index/TermMeta.h"

FX_NS_DEF(index);

class TermInfo : public TermMeta
{
public:
    TermInfo(void)
        : m_nPostingPointer(0)
    {
    }

    TermInfo(df_t df, offset_t dp, ctf_t ctf = 0)
        : TermMeta(df, ctf)
        , m_nPostingPointer(dp)
    {
    }

    ~TermInfo(void)
    {
    }

public:
    offset_t getPostingPointer() const;

    void set(df_t df, offset_t dp, ctf_t ctf = 0)
    {
        m_docFreq = df;
        m_nPostingPointer = dp;
        m_totalTermFreq = ctf;
    }

protected:
    offset_t m_nPostingPointer;
};

inline offset_t TermInfo::getPostingPointer() const 
{
    return m_nPostingPointer;
}

FX_NS_END

#endif

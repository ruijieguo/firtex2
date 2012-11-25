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
// Created	: 2010-07-11 12:06:51

#ifndef __FX_TERMSCOREFEATURE_H
#define __FX_TERMSCOREFEATURE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(search);

class TermScoreFeature
{
public:
    TermScoreFeature() : m_fTermFreq(0.0) {}
    TermScoreFeature(float tf) : m_fTermFreq(tf) {}
    ~TermScoreFeature() {}

public:
    /** Get/set term freq */
    void setTermFreq(float tf) { m_fTermFreq = tf;}
    float getTermFreq() const { return m_fTermFreq;}

protected:
    float m_fTermFreq;
};

FX_NS_END

#endif //__FX_TERMSCOREFEATURE_H

//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : Ruijie Guo
// Email        : ruijieguo@gmail.com
// Created      : 2011-03-19 09:19:38

#ifndef __FX_SEGMENTER_H
#define __FX_SEGMENTER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/analyzer/TokenView.h"

FX_NS_DEF(analyzer);

class Segmenter
{
public:
    virtual ~Segmenter() {}

public:
    /**
     * Load lexicon and initialize
     * @param sLexiconFile file path of lexicon
     */
    virtual void init(const tstring& sLexiconFile) = 0;

    /**
     * Segment text to tokens
     * @param pTokenView result segmented tokens
     * @param szText text need to segment
     * @param length of text in bytes
     */
    virtual void segment(TokenViewPtr& pTokenView, 
                         const char* szText,
                         size_t length) const = 0;

    /**
     * Clone a new segmenter
     */
    virtual Segmenter* clone() const = 0;
};

FX_NS_END

#endif //__FX_SEGMENTER_H

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
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2007/1/28
//
#ifndef _SNOWBALLFILTER_H
#define _SNOWBALLFILTER_H

#include "firtex/utility/StringReader.h"
#include "firtex/analyzer/TokenFilter.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(analyzer)

class SnowballWrapper;
class SnowballFilter : public TokenFilter
{
public:
    DECLARE_FILTER_CREATOR(SnowballFilter, _T(".snowball"));

public:
    SnowballFilter();
    SnowballFilter(const SnowballFilter& src);
    SnowballFilter(const tstring& param);

    /**
     * constructor
     * @param sLanguage language,NULL for English
     * @param sEnc encode, NULL for utf-8
     */
    SnowballFilter(const tstring& sLanguage, const tstring& sEnc);

    /**
     * constructor
     * @param sLanguage language,NULL for English
     * @param sIrregularWords irregular words file
     * @param sEnc encode, NULL for utf-8
     * @param pPreFilter filter before this filter
     */
    SnowballFilter(const tstring& sLanguage, const tstring& sIrregularWords, 
                   const tstring& sEnc, TokenFilter* pPreFilter);
    
    virtual ~SnowballFilter(void);

public:
    /**
     * get identifier of Filter
     */
    const tstring& getIdentifier() const;

    /**
     * set parameter for token filter
     */
    void setParam(const tstring& sParam);
    
    /**
     * Clone a new object
     */
    TokenFilter* clone() const;

protected:
    /**
     * \if EN
     * Filter interface, must implement by sub class
     * @param tokenSource token source for filter
     * \endif
     */
    void filterInternal(TokenSourcePtr& tokenSource) const;

protected:
    SnowballWrapper* m_pSnowball;
};

FX_NS_END

#endif


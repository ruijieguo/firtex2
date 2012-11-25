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
// Created	: 2007/3/8
//

#ifndef __LOWERCASEANTEFILTER_H
#define __LOWERCASEANTEFILTER_H

#include "TokenFilter.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(analyzer)

class LowercaseFilter : public TokenFilter
{
public:
    DECLARE_FILTER_CREATOR(LowercaseFilter, _T(".lowercase"));

public:
    LowercaseFilter(void);
    LowercaseFilter(const LowercaseFilter& src);
    LowercaseFilter(TokenFilter* pPreFilter);
    virtual ~LowercaseFilter(void);

public:
    /**
     * get identifier of Filter
     */
    const tstring& getIdentifier() const;

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

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif

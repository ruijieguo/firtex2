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
// Created	: 2005/11/24
//
#ifndef _STANDARDSTOPFILTER_H
#define _STANDARDSTOPFILTER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "TokenFilter.h"
#include "firtex/utility/Singleton.h"
#include <set>

FX_NS_DEF(analyzer);

class StandardStopFilter : public TokenFilter
{
public:
    DECLARE_FILTER_CREATOR(StandardStopFilter, _T(".stopword"));

public:
    const static int STOPWORD_ID = -1;

public:
    StandardStopFilter();
    StandardStopFilter(const StandardStopFilter& src);
    virtual ~StandardStopFilter(void);

public:
    /**
     * Init filter from stop word file
     */
    void init(const tstring& stopfile);

    /**
     * set parameter for token filter
     */
    void setParam(const tstring& sParam);

    /**
     * Load stop words from file
     */
    size_t loadFile(const tstring& filename);

    /**
     * Load stop words words list
     */
    size_t loadWords(const std::string& sWords);

    /**
     * Clone a new object
     */
    TokenFilter* clone() const;

protected:
    const tstring& getIdentifier() const;
    void filterInternal(TokenSourcePtr& tokenSource) const;

protected:
    typedef std::set<termid_t> StopWordSet;
    StopWordSet m_stopwords;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif

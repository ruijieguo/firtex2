//
// Copyright(C) 2005--2009 FirteX Develop Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@gmail.com
// Created	: 2009/3/17
//
#ifndef __SYNONYMFILTER_H
#define __SYNONYMFILTER_H

#include "TokenFilter.h"
#include "firtex/utility/Singleton.h"
#include "SynonymMap.h"

FX_NS_DEF(analyzer);

class SynonymFilter : public TokenFilter
{
public:
    DECLARE_FILTER_CREATOR(SynonymFilter, _T(".synonym"));

public:
    SynonymFilter(void);
    SynonymFilter(const SynonymFilter& src);
    SynonymFilter(TokenFilter* pPreFilter);			
    virtual ~SynonymFilter(void);

public:
    /**
     * set parameter for token filter
     */
    void setParam(const tstring& sParam);

    /**
     * get identifier of Filter
     */
    const tstring& getIdentifier() const {return SynonymFilter::Creator::instance()->getIdentifier();};

    /**
     * load synonym words from file	 
     */
    void loadSynonyms(const tstring& szSynFile);

    /**
     * add synonyms
     * @param szSynonyms synonym words splited by ','	 
     */
    SynonymMap& getSynonymMap() {return m_synonyms;}

    /**
     * Clone a new object
     */
    TokenFilter* clone() const;

protected:
    void filterInternal(TokenSourcePtr& tokenSource) const;

private:
    SynonymMap m_synonyms;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif

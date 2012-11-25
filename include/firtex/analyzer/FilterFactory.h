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
// Created	: 2009/5/19
//
#ifndef _FILTERFACTORY_H
#define _FILTERFACTORY_H

#include "firtex/common/StdHeader.h"
#include "Token.h"
#include "TokenFilter.h"
#include "firtex/utility/Singleton.h"
#include "firtex/thread/SynchronizedObject.h"
#include <map>

FX_NS_DEF(analyzer)

class FilterFactory : public FX_NS(thread)::SynchronizedObject
                    , public FX_NS(utility)::Singleton<FilterFactory>
{
public:
    typedef std::map<tstring, TokenFilter::Creator*> CreatorMap;
    typedef std::vector<tstring>  FilterList;

protected:
	FilterFactory(void);
	~FilterFactory(void);
public:			
	/**
	 * create analyzer by identifier
	 * @param sIdentifier identifier of analyzer
	 * @param sParam parameter of analyzer
	 * @return analyzer
	 */
	TokenFilter* createFilter(const tstring& sIdentifier, const tstring& sParam);
public:
	/**
	 * register a creator of TokenFilter
	 * @param pCreator creator of TokenFilter
	 */
	void registerCreator(TokenFilter::Creator* pCreator);	

	/** 
	 * get registered TokenFilters
	 * @return registered TokenFilters
	 */
	FilterList getRegisteredFilters();

protected:
	/**
	 * get a registered creator
	 * @param _identifier identifier of Tokenfilter
	 * @return creator of TokenFilter
	 */
    TokenFilter::Creator* getCreator(const tstring& _identifier);

	/**
	 * get creator map			 
	 */
	CreatorMap& getCreatorMap(){return m_creatorsMap;}

protected:
	CreatorMap	m_creatorsMap;

	DECLARE_LAZY_SINGLETON(FilterFactory);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif

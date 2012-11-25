//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2005/11/20
//
#ifndef _ANALYZERFACTORY_H
#define _ANALYZERFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "Token.h"
#include "TokenFilter.h"
#include "Analyzer.h"
#include "firtex/utility/Singleton.h"
#include "firtex/thread/SynchronizedObject.h"
#include <map>

FX_NS_DEF(analyzer)

typedef std::map<std::string,FX_NS(analyzer)::Analyzer::Creator*> CreatorMap;
typedef std::vector<std::string> AnalyzerList;

class AnalyzerFactory : public FX_NS(thread)::SynchronizedObject,
                        public FX_NS(utility)::Singleton<AnalyzerFactory>
{
protected:
    AnalyzerFactory(void);
    ~AnalyzerFactory(void);

public:
    /**
     * create analyzer by identifier
     * @param sIdentifier identifier of analyzer
     * @param sParam parameter of analyzer
     * @return analyzer
     */
    Analyzer* createAnalyzer(const std::string& sIdentifier, const std::string& sParam);

public:
    /**
     * register a creator of collection
     * @param pCreator creator of collection
     */
    void registerCreator(Analyzer::Creator* pCreator);

    /** 
     * get registered analyzers
     * @return registered analyzers 
     */
    AnalyzerList getRegisteredAnalyzers();

protected:
    /**
     * get a registered creator
     * @param sIdentifier identifier of collection
     * @return creator of collection
     */
    Analyzer::Creator* getCreator(const std::string& sIdentifier);

    /**
     * get creator map
     */
    CreatorMap&	getCreatorMap() {return m_creatorsMap;}

protected:
    CreatorMap m_creatorsMap;

    DECLARE_LAZY_SINGLETON(AnalyzerFactory);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif

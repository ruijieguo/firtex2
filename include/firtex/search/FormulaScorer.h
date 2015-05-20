//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2013-01-01 15:43:08

#ifndef __FX_FORMULASCORER_H
#define __FX_FORMULASCORER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Scorer.h"

FX_NS_DEF(search);

class FormulaScorer : public Scorer
{
public:
    FormulaScorer();
    ~FormulaScorer();

public:
    /**
     * Begin to execute a query
     * @param pProvider providing features for ranking
     */
    virtual void beginQuery(const FeatureProviderPtr& pProvider,
                            const QueryFeature& queryFeature);
    
    /**
     * Score matched documents
     * @param matchedDocs the matched doc set
     */
    virtual void score(MatchedDocSet& matchedDocs);

    /**
     * Query executing is ended
     */
    virtual void endQuery();

    /**
     * Set parameter
     */
    virtual void setParameter(const std::string& sParam);

private:
    std::string m_sFormula;
private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_FORMULASCORER_H

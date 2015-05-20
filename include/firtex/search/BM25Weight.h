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
// Created : 2013-01-24 21:24:59

#ifndef __FX_BM25WEIGHT_H
#define __FX_BM25WEIGHT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/IndexFeature.h"
#include "firtex/search/TermQueryFeature.h"
#include <math.h>

FX_NS_DEF(search);

class BM25Weight
{
public:
    BM25Weight() : m_fIdf(0.0), m_fBoost(0.0) {}

public:
    void init(const IndexFeature& indexFeature,
              const TermQueryFeature* pTqFeature)
    {
        if (pTqFeature->getQueryType() == TermQueryFeature::QT_PHRASE)
        {
            size_t uNumSuc = pTqFeature->getNumSuccessors();
            FX_TRACE("number successors: %u", (uint32_t)uNumSuc);
            for (size_t i = 0; i < uNumSuc; ++i)
            {
                BM25Weight w;
                w.init(indexFeature, (pTqFeature + i + 1));
                m_fIdf += w.getIdf();
            }
            FX_TRACE("result idf: %f", m_fIdf);
        }
        else 
        {
            df_t df = pTqFeature->getDocFreq();
            if (df > 0)
            {
                float fNumDocs = (float)indexFeature.getDocCount();
                m_fIdf = (score_t)(log(fNumDocs / (float)(df + 1)) + 1.0);
                FX_TRACE("idf: %f, df: %d, total docs: %f",
                        m_fIdf, df, fNumDocs);
            }
        }
        m_fBoost = pTqFeature->getBoost();
    }

    float getIdf() const {return m_fIdf;}
    float getBoost() const {return m_fBoost;}

protected:
    float m_fIdf;
    float m_fBoost;

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_BM25WEIGHT_H

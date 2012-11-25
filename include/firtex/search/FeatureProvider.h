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
// Created	: 2010-07-05 23:35:08

#ifndef __FX_FEATUREPROVIDER_H
#define __FX_FEATUREPROVIDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Similarity.h"
#include "firtex/search/IndexFeature.h"
#include "firtex/search/QueryFeature.h"

FX_NS_DEF(search);

class FeatureProvider
{
public:
    FeatureProvider();
    ~FeatureProvider();

public:
    /**
     * Initialize
     */
    void init(const FX_NS(index)::IndexReaderPtr& pIndexReader,
              const Similarity* pSimilarity);

    /**
     * Return index feature
     */
    const IndexFeature& getIndexFeature() const;

protected:
    IndexFeature m_indexFeature;
    const Similarity* m_pSimilarity;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(FeatureProvider);

/////////////////////////////////////////////////////
//
inline const IndexFeature& FeatureProvider::getIndexFeature() const
{
    return m_indexFeature;
}

typedef FX_NS(common)::SharedPtr<FeatureProvider> FeatureProviderPtr;

FX_NS_END

#endif //__FX_FEATUREPROVIDER_H

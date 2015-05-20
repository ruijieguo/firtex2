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
// Created : 2013-01-03 11:09:23

#ifndef __FX_FEATURECONTEXT_H
#define __FX_FEATURECONTEXT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(search);

class FeatureProvider;
class QueryFeature;
class DocScoreFeature;

struct FeatureContext
{
    FeatureContext(const FeatureProvider* p, const QueryFeature* f)
        : featureProvider(p), queryFeature(f)
    {
    }

    const FeatureProvider* featureProvider;
    const QueryFeature* queryFeature;
};

FX_NS_END

#endif //__FX_FEATURECONTEXT_H

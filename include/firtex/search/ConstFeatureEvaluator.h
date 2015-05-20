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
// Created : 2013-01-02 11:36:01

#ifndef __FX_CONSTFEATUREEVALUATOR_H
#define __FX_CONSTFEATUREEVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ConstExprEvaluator.h"

FX_NS_DEF(search);

typedef ConstExprEvaluator<uint32_t> DocAvgLengthFeatureEvaluator;
typedef ConstExprEvaluator<uint64_t> DocCountFeatureEvaluator;
typedef ConstExprEvaluator<uint32_t> FieldAvgLengthFeatureEvaluator;

FX_NS_END

#endif //__FX_CONSTFEATUREEVALUATOR_H

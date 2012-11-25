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
// Created	: 2006/5/9
//
#include <math.h>
#include "firtex/search/DefaultSimilarity.h"


FX_NS_DEF(search)

SETUP_STREAM_LOGGER(search, DefaultSimilarity);

DefaultSimilarity::DefaultSimilarity(void)
{
}

DefaultSimilarity::~DefaultSimilarity(void)
{
}

float DefaultSimilarity::tf(float freq)
{
	return (float) sqrt(freq);
}

float DefaultSimilarity::idf(tf_t nDocFreq, df_t nNumDocs)
{
	return (float) (log((double)nNumDocs / (double) (nDocFreq + 1)) + 1.0);
}

float DefaultSimilarity::sloppyFreq(int32_t nDistance)
{
	return 1.0f / (nDistance + 1);
}

float DefaultSimilarity::lengthNorm(tf_t nNumTerms)
{
	if(nNumTerms == 0)
		return 0.0f;
	return (float)(1.0f / sqrt((float)nNumTerms));
}

float DefaultSimilarity::queryNorm(float fSumOfSquaredWeights)
{
	if(fSumOfSquaredWeights == 0)
		return 0.0f;
	return (float)(1.0f / sqrt((float)fSumOfSquaredWeights));
}

float DefaultSimilarity::coord(int32_t nOverlap, int32_t nMaxOverlap)
{
	return (float)((float)nOverlap / (float)nMaxOverlap);
}

FX_NS_END

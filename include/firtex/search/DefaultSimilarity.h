//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: 郭瑞杰(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2006/5/9
//
#ifndef _DEFAULT_SIMILARITY_H
#define _DEFAULT_SIMILARITY_H

#include "Similarity.h"

FX_NS_DEF(search)

/**
 * \if CN
 * Similarity的默认实现
 * \endif
 */
class DefaultSimilarity :	public Similarity
{
public:
	DefaultSimilarity(void);
	virtual ~DefaultSimilarity(void);
public:				

	/**
	 * \if EN
	 * compute tf
	 * @param freq term frequency
	 * @return tf value
	 * \endif
	 */
	float tf(float freq);

	/**
	 * \if EN
	 * compute idf
	 * @param nDocFreq document frequency
	 * @param nNumDocs total documents
	 * @return idf value
	 * \endif
	 */
	float idf(df_t nDocFreq, df_t nNumDocs);

	float sloppyFreq(int32_t nDistance);

	float lengthNorm(tf_t nNumTerms);

	float queryNorm(float fSumOfSquaredWeights);

	float coord(int32_t nOverlap, int32_t nMaxOverlap);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif

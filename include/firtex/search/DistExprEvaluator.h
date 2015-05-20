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
// Created : 2012-12-15 16:42:26

#ifndef __FX_DISTEXPREVALUATOR_H
#define __FX_DISTEXPREVALUATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ExprEvaluator.h"
#include <cmath>

FX_NS_DEF(search);

class DistExprEvaluator : public ExprEvaluator
{
public:
    DistExprEvaluator(ExprEvaluator* pLat1Expr,
                      ExprEvaluator* pLon1Expr,
                      ExprEvaluator* pLat2Expr,
                      ExprEvaluator* pLon2Expr);

    ~DistExprEvaluator() 
    {
        delete m_pLat1Expr;
        delete m_pLon1Expr;
        delete m_pLat2Expr;
        delete m_pLon2Expr;
    };

public:
    /**
     * Evaluate the float value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    float evaluateFloat(const ScoreDoc& scoredDoc);

    /**
     * Evaluate the double value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    double evaluateDouble(const ScoreDoc& scoredDoc);

    /**
     * Evaluate the int32 value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    int32_t evaluateInt32(const ScoreDoc& scoredDoc);

    /**
     * Evaluate the int64 value of expression
     * @param scoredDoc scored doc to evaluate
     * @return result value
     */
    int64_t evaluateInt64(const ScoreDoc& scoredDoc);

private:
    /**
     * Computes distance between two given points specified by 
     * their coordinates. Both latitudes and longitudes must be in 
     * radians and the result will be in meters.
     */
    static inline float calcDistance(float fLat1, float fLon1, float fLat2, float fLon2);

    static inline double mySqr(double v);
private:
    ExprEvaluator* m_pLat1Expr;
    ExprEvaluator* m_pLon1Expr;
    ExprEvaluator* m_pLat2Expr;
    ExprEvaluator* m_pLon2Expr;

private:
    DECLARE_LOGGER();
};

////////////////////////////////////
///
inline float DistExprEvaluator::calcDistance(float fLat1, float fLon1,
        float fLat2, float fLon2)
{
    static const double R = 6384000;
    double dLat = (double)(fLat1 - fLon2);
    double dLon = (double)(fLon1 - fLon2);
    double a = mySqr(std::sin(dLat/2)) + std::cos(fLat1) * 
               std::cos(fLat2) * mySqr(std::sin(dLon/2));
    double c = 2 * asin(MIN(1, sqrt(a)));
    return (float)(R*c);
}

inline double DistExprEvaluator::mySqr(double v)
{
    return v * v;
}

FX_NS_END

#endif //__FX_DISTEXPREVALUATOR_H

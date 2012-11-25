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
// Created	: 2007/6/16
//
#ifndef __NUMERICANALYZER_H
#define __NUMERICANALYZER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Analyzer.h"
#include "firtex/utility/Number.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(analyzer)


template<typename T>
class NumericAnalyzer : public Analyzer
{		
public:
    typedef T NumberType;

public:


public:
    NumericAnalyzer(void);
    NumericAnalyzer(const NumericAnalyzer<T>& src);
    virtual ~NumericAnalyzer(void);

public:		
    /**
     * get identifier of analyzer
     * @return the identifier
     */
    const tstring getIdentifier() const;

    /**
     * Clone a new object
     */
    Analyzer* clone() const;

protected:
    void doTokenize(TokenSourcePtr& tokenSource) const;

public:
    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,bool* pNumBuf,size_t nMaxNum);

    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,int8_t* pNumBuf,size_t nMaxNum);

    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,uint8_t* pNumBuf,size_t nMaxNum);

    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,int16_t* pNumBuf,size_t nMaxNum);

    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,uint16_t* pNumBuf,size_t nMaxNum);

    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,int32_t* pNumBuf,size_t nMaxNum);

    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,uint32_t* pNumBuf,size_t nMaxNum);

    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,int64_t* pNumBuf,size_t nMaxNum);

    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,uint64_t* pNumBuf,size_t nMaxNum);

    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,float* pNumBuf,size_t nMaxNum);

    /**
     * convert string to bool value
     * @param str string
     * @param pNumBuf number's buffer
     * @param nMaxNum max numbers the buffer can contain.
     * @return number of values
     */
    static int32_t parse(const tchar* str,double* pNumBuf,size_t nMaxNum);
};


template<typename T>
class NumericAnalyzerCreator : public Analyzer::Creator,
                               public FX_NS(utility)::Singleton<NumericAnalyzerCreator<T> >
{
public:
    NumericAnalyzerCreator();

public:
    virtual ~NumericAnalyzerCreator(){}

public:
    /**
     * create a collection object
     * @return create a analyzer
     */
    virtual Analyzer* create() const {return new NumericAnalyzer<T>();}

//		DECLARE_LAZY_SINGLETON(NumericAnalyzer::Creator);
};

/////////////////////////////////////////////////////////////////////
//


template<typename T>
NumericAnalyzer<T>::NumericAnalyzer(void)
{
    setMaxTokensToAnalyze(1);
}

template<typename T>
NumericAnalyzer<T>::NumericAnalyzer(const NumericAnalyzer<T>& src)
    : Analyzer(src)
{
}


template<typename T>
NumericAnalyzer<T>::~NumericAnalyzer(void)
{
}

template<typename T>
const tstring NumericAnalyzer<T>::getIdentifier() const
{
    return NumericAnalyzerCreator<T>::instance()->getIdentifier();

}

template<typename T>
Analyzer* NumericAnalyzer<T>::clone() const
{
    return new NumericAnalyzer<T>(*this);
}

template<typename T>
void NumericAnalyzer<T>::doTokenize(TokenSourcePtr& tokenSource) const
{
    //TODO:
    assert(false);
//     TokenViewPtr pInputTokens = tokenSource->getLastView();
//     TokenViewPtr pOutputTokens = tokenSource->getCustomView(_T("datetime_view"));
//     if (!pOutputTokens)
//     {
//         pOutputTokens.assign(new TypedTokenView<T>(getMaxTokensToAnalyze()));
//         tokenSource->addCustomView(_T("datetime_view"), pOutputTokens);
//     }

//     char* pText = pInputTokens->getTokenBuffer();
// //    size_t length = pInputTokens->getTokenBufferSize();
//     void* pTokensBuf = (void*)pOutputTokens->getTokenBuffer();
//     size_t nMaxTokens = pOutputTokens->getMaxTokens();

//     int32_t nNumTokens = parse((const tchar*)pText, (T*)pTokensBuf, nMaxTokens);
//     if(nNumTokens > 0)
//     {
//         pOutputTokens->setNumTokens(nNumTokens);
//     }
}			

template<typename T>
int32_t NumericAnalyzer<T>::parse(const tchar* str, bool* pNumBuf, size_t nMaxNum)
{
    return 0;
}

template<typename T>
int32_t NumericAnalyzer<T>::parse(const tchar* str, int8_t* pNumBuf, size_t nMaxNum)
{
    return 0;
}

template<typename T>
int32_t NumericAnalyzer<T>::parse(const tchar* str, uint8_t* pNumBuf, size_t nMaxNum)
{
    return 0;
}

template<typename T>
int32_t NumericAnalyzer<T>::parse(const tchar* str, int16_t* pNumBuf, size_t nMaxNum)
{
    return 0;
}

template<typename T>
int32_t	NumericAnalyzer<T>::parse(const tchar* str, uint16_t* pNumBuf, size_t nMaxNum)
{
    return 0;
}

template<typename T>
int32_t NumericAnalyzer<T>::parse(const tchar* str, int32_t* pNumBuf, size_t nMaxNum)
{
    char* pTmp;
    *pNumBuf = (int32_t)strtol(str,&pTmp,10);
    return 1;
}

template<typename T>
int32_t NumericAnalyzer<T>::parse(const tchar* str, uint32_t* pNumBuf, size_t nMaxNum)
{
    return 0;
}

template<typename T>
int32_t NumericAnalyzer<T>::parse(const tchar* str, int64_t* pNumBuf, size_t nMaxNum)
{
    char* pTmp;
    *pNumBuf = (int64_t)strtol(str, &pTmp, 10);
    return 1;
}

template<typename T>
int32_t NumericAnalyzer<T>::parse(const tchar* str, uint64_t* pNumBuf, size_t nMaxNum)
{
    return 0;
}

template<typename T>
int32_t NumericAnalyzer<T>::parse(const tchar* str, float* pNumBuf, size_t nMaxNum)
{
    return 0;
}

template<typename T>
int32_t NumericAnalyzer<T>::parse(const tchar* str, double* pNumBuf, size_t nMaxNum)
{
    return 0;
}

FX_NS_END

#endif

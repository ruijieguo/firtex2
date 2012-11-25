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
// Email	: ruijieguo@gmail.com
// Created	: 2005/11/20
//

#ifndef __FX_ANALYZER_H
#define __FX_ANALYZER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/Document.h"
#include "firtex/utility/Reader.h"
#include "firtex/analyzer/TokenView.h"
#include "firtex/analyzer/TokenFilter.h"
#include "firtex/document/DocumentType.h"

FX_NS_DEF(analyzer)

#define DECLARE_ANALYZER_CREATOR(AnalyzerClass, AnalyzerName)           \
    class Creator : public Analyzer::Creator                            \
                  , public FX_NS(utility)::Singleton<AnalyzerClass::Creator> \
    {                                                                   \
    protected:                                                          \
        Creator()                                                       \
        {                                                               \
            m_identifier += _T(".");                                    \
            m_identifier += AnalyzerName;                               \
        }                                                               \
        virtual ~Creator(){}                                            \
    public:                                                             \
        virtual Analyzer* create() const {return new AnalyzerClass;}    \
        DECLARE_LAZY_SINGLETON(AnalyzerClass::Creator);                 \
    };

class Analyzer
{
public:
    class Creator
    {
    protected:
        Creator() : m_identifier(_T("analyzer"))
        {}
        virtual ~Creator(){}

    public:
        /**
         * Create an analyzer object
         * @return create a analyzer
         */
        virtual Analyzer* create() const = 0;

        /** 
         * Return the identifier of collection
         */
        virtual const std::string getIdentifier() const {return m_identifier;}

    protected:
        std::string m_identifier;
    };

public:
    Analyzer();
    Analyzer(const Analyzer& src);
    virtual ~Analyzer(void);

public:
    /**
     * attach a post-filter, generally, it is a stop word filter
     * @param[in] pPostTokenFilter post-filter
     * @see detachPostFilter(), TokenFilter
     */
    void attachPostFilter(TokenFilter* pPostTokenFilter);

    /**
     * detach a post-filter
     * @return post-filter,NULL if there is no post-filter
     * @see attachPostFilter(TokenFilter*), TokenFilter
     */
    TokenFilter* detachPostFilter();

    /**
     * attach a ante-filter, generally, it is a stemming filter
     * @param[in] pAnteTokenFilter ante-filter
     * @see detachAnteFilter(), TokenFilter
     */
    void attachAnteFilter(TokenFilter* pAnteTokenFilter);

    /**
     * detach a ante-filter
     * @return ante-filter,NULL if there is no ante-filter
     * @see attachAnteFilter(TokenFilter*), TokenFilter
     */
    TokenFilter* detachAnteFilter();

    /**
     * detach a ante-filter by identifier
     * @return ante-filter,NULL if there is no ante-filter
     * @see attachAnteFilter(TokenFilter*), TokenFilter
     */
    TokenFilter* detachAnteFilter(const std::string& szIdentifier);

public:
    /**
     * Tokenize text to tokens
     * @param sText text to analyze
     * @return tokenized tokens 
     */
    TokenViewPtr tokenize(const std::string& sText) const;

    /**
     * Tokenize text to tokens
     * @param szText text to analyze
     * @return tokenized tokens 
     */
    TokenViewPtr tokenize(const char* szText, size_t length) const;

    /**
     * Tokenize text to tokens
     * @param tokenSource original text is in original view
     * @return tokenized tokens 
     */
    TokenViewPtr tokenize(TokenSourcePtr& tokenSource) const;

public:
    /**
     * set max tokens to analyze
     * @param[in] nMaxTokensToAnalyze max tokens to analyze
     */
    inline void setMaxTokensToAnalyze(int32_t nMaxTokensToAnalyze)
    {
        m_nMaxTokensToAnalyze = nMaxTokensToAnalyze;
    }

    /**
     * get max tokens to analyze
     * @return max tokens to analyze
     */
    inline int32_t getMaxTokensToAnalyze() const {return m_nMaxTokensToAnalyze;}

public:
    /**
     * get identifier of analyzer
     * @return the identifier
     */
    virtual const std::string getIdentifier() const = 0;

    /**
     * Clone a new object
     */
    virtual Analyzer* clone() const = 0;

public:
    /**
     * Initialize or load dictionary
     */
    virtual void init() {}

    /**
     * Set parameter for analyzer
     * @param[in] sParam parameter for analyzer. format: param_name=param_value,...
     */
    virtual void setParam(const std::string& sParam);

protected:
    /**
     * Do tokenize operate, implement by sub-class
     * @param tokenSource token source
     */
    virtual void doTokenize(TokenSourcePtr& tokenSource) const = 0;

protected:
    TokenFilter* m_pAnteTokenFilter;
    TokenFilter* m_pPostTokenFilter;
    int32_t m_nMaxTokensToAnalyze;

protected:
    const static int32_t DEFAULT_MAX_BYTES_TO_ANALYZE = 10 * 1024 * 1024;///10M;
};

DEFINE_TYPED_PTR(Analyzer);

/////////////////////////////////////////////////////////////
//
inline TokenViewPtr Analyzer::tokenize(const std::string& sText) const
{
    return tokenize(sText.c_str(), sText.length());
}

FX_NS_END

#endif

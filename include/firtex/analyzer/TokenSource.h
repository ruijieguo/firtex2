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
// Created	: 20010/01/30
//

#ifndef __TOKENSOURCE_H
#define __TOKENSOURCE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "TokenView.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(analyzer)

class TokenSource
{
public:
    const static size_t DEFAULT_ORIGINAL_TEXT_SIZE = 512 * 1024;

public:
    typedef std::map<tstring, TokenViewPtr> ViewMap;
    typedef ViewMap::const_iterator ViewIterator;

public:
    TokenSource(void);
    ~TokenSource(void);

public:
    /**
     * set original text of token source
     * @param szText original text
     * @param length length of szText
     *
     */
    void setOriginalView(const char* szText, size_t length);

    /**
     * set original text of token source
     * @param pTokenView original token buffer
     *
     */
    void setOriginalView(const TokenViewPtr& pTokenView);

    /**
     * Get original view by view id
     */
    TokenViewPtr getOriginalView() const { return m_pOrgView;}

    /**
     * Get view by view id
     */
    TokenViewPtr getLastView() const { return m_pLastView;}

    /**
     * Get view by view id
     */
    void setLastView(const TokenViewPtr& pTokenView) { m_pLastView = pTokenView;}

    /**
     * Add custom view
     */
    void addCustomView(const tstring& sViewName,
                       const TokenViewPtr& pTokenView);

    /**
     * Get cumstom view by name
     */
    TokenViewPtr acquireCustomView(const tstring& sViewName);

    /**
     * Clear all token views
     */
    void reset();

private:
    ViewMap m_customViewMap;
    TokenViewPtr m_pOrgView;
    TokenViewPtr m_pLastView;

private:
    TokenSource(const TokenSource& clone);
    TokenSource& operator = (const TokenSource& clone);
};

DEFINE_TYPED_PTR(TokenSource);

FX_NS_END

#endif

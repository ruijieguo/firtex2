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
// Created	: 2011/4/5
//
#ifndef __COMTOKENSOURCE_H
#define __COMTOKENSOURCE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/extension/plugin/FirteXCOM.h"
#include "firtex/extension/plugin/ComObject.h"
#include "firtex/analyzer/TokenSource.h"

FX_NS_DEF(plugin);

class ComTokenSource : public ComObject
{
public:
    ComTokenSource(FX_NS(analyzer)::TokenSource* pTokens);
    virtual ~ComTokenSource(void);
public:
    static ITokenSource* wrap(FX_NS(analyzer)::TokenSource* pTokens);
    static FX_NS(analyzer)::TokenSource* unwrap(ITokenSource* pInterface);

public:
    ITokenSource* getInterface();

public:
    BEGIN_INTERFACE_PART(TokenSource, ITokenSource)

    FX_STDMETHOD(acquireCustomView)(/* [in] */ FX_NS(com)::FX_BSTR viewName, 
                                    /* [out][retval */ ITokenView** ppTokenView);
    FX_STDMETHOD(getOriginalView)(/* [out][retval */ ITokenView** ppTokenView);
    FX_STDMETHOD(getLastView)(/* [out][retval */ ITokenView** ppTokenView);
    FX_STDMETHOD(reset)();

    END_INTERFACE_PART(TokenSource);

private:
    FX_NS(analyzer)::TokenSource* m_pTokens;
};

FX_NS_END

#endif

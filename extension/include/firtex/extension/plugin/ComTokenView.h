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
// Created	: 2006/7/9
//
#ifndef __COMTOKENVIEW_H
#define __COMTOKENVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FirteXCOM.h"
#include "ComObject.h"
#include "firtex/analyzer/TokenView.h"

FX_NS_DEF(plugin);

class ComTokenView : public ComObject
{
public:
    ComTokenView(FX_NS(analyzer)::TokenView* pTokens);
    virtual ~ComTokenView(void);
public:
    static ITokenView* wrap(FX_NS(analyzer)::TokenView* pTokens);
    static FX_NS(analyzer)::TokenView* unwrap(ITokenView* pInterface);

public:
    BEGIN_INTERFACE_PART(TokenView, ITokenView)

    FX_STDMETHOD(addToken)(/* [in] */ FX_NS(com)::FX_VARIANT* pTokenVar, 
                           /* [in] */ FX_NS(com)::FX_INT32 nPosInc,
                           /* [in] */ FX_NS(com)::FX_INT32 nStartOffset,
                           /* [in] */ FX_NS(com)::FX_INT32 nEndOffset,
                           /* [in] */ TOKENTYPE tokenType);

    FX_STDMETHOD(getNumTokens)(/* [out] [retval] */ FX_NS(com)::FX_INT32* pNumTokens);
    FX_STDMETHOD(clear)();

    END_INTERFACE_PART(TokenView);

private:
    FX_NS(analyzer)::TokenView* m_pTokens;
};

FX_NS_END

#endif

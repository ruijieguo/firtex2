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
// Created	: 2006/7/9
//
#include "firtex/extension/plugin/ComTokenView.h"
#include "firtex/analyzer/Token.h"

FX_NS_USE(com);
FX_NS_USE(analyzer);

FX_NS_DEF(plugin);

ComTokenView::ComTokenView(FX_NS(analyzer)::TokenView* pTokens) 
  : m_pTokens(pTokens)
{
}

ComTokenView::~ComTokenView(void)
{
    m_pTokens = NULL;
}

ITokenView* ComTokenView::wrap(FX_NS(analyzer)::TokenView* pTokens)
{
    ComTokenView* pWrap = new ComTokenView(pTokens);
    CLASS_TO_INTERFACE(ComTokenView,TokenView,pWrap,pXThis);
    ITokenView* pCom = NULL;
    pXThis->QueryInterface(FX_NS(plugin)::ITokenView::iid, (void**)&pCom);
    return pCom;
}

FX_NS(analyzer)::TokenView* ComTokenView::unwrap(ITokenView* pInterface)
{
    if ( pInterface == NULL ) return NULL;
    INTERFACE_TO_CLASS(ComTokenView, TokenView, pInterface, pWrapper)
    return (FX_NS(analyzer)::TokenView*)pWrapper->m_pTokens;
}
//////////////////////////////////////////////////////////////////////////
//ComReader ITokenView
IMPLEMENT_UNKNOWN(ComTokenView, TokenView)

FX_STDMETHODIMP ComTokenView::XTokenView::addToken(
        /* [in] */ FX_NS(com)::FX_VARIANT* pTokenVar, 
        /* [in] */ FX_NS(com)::FX_INT32 nPosInc,
        /* [in] */ FX_NS(com)::FX_INT32 nStartOffset,
        /* [in] */ FX_NS(com)::FX_INT32 nEndOffset,
        /* [in] */ TOKENTYPE tokenType)
{
    FX_METHOD_PROLOGUE( ComTokenView, TokenView);
    if (pTokenVar->vt != FX_VT_STRING)
    {
        return FX_S_FALSE;
    }
    
    pThis->m_pTokens->addToken(pTokenVar->sVal.str, pTokenVar->sVal.length,
                               (int32_t)nPosInc, (int32_t)nStartOffset,
                               (int32_t)nEndOffset, (Token::TokenType)tokenType);
    return FX_S_OK;
}
 
FX_STDMETHODIMP ComTokenView::XTokenView::getNumTokens(
        /* [out] [retval] */ FX_NS(com)::FX_INT32* pNumTokens)
{
    FX_METHOD_PROLOGUE( ComTokenView, TokenView);

    *pNumTokens = (FX_NS(com)::FX_INT32)pThis->m_pTokens->getNumTokens();
    return FX_S_OK;
}

FX_STDMETHODIMP ComTokenView::XTokenView::clear()
{
    FX_METHOD_PROLOGUE( ComTokenView, TokenView)

    pThis->m_pTokens->clear();
    return FX_S_OK;
}

FX_NS_END

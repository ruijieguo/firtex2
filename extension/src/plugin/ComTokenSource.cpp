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
#include "firtex/extension/plugin/ComTokenSource.h"
#include "firtex/extension/plugin/ComTokenView.h"
#include "firtex/extension/com/ComPtr.h"
#include "firtex/analyzer/TokenView.h"

FX_NS_USE(com);
FX_NS_USE(analyzer);

FX_NS_DEF(plugin);

ComTokenSource::ComTokenSource(FX_NS(analyzer)::TokenSource* pTokens) : m_pTokens(pTokens)
{
}

ComTokenSource::~ComTokenSource(void)
{
    m_pTokens = NULL;
}

ITokenSource* ComTokenSource::wrap(FX_NS(analyzer)::TokenSource* pTokens)
{
    ComTokenSource* pWrap = new ComTokenSource(pTokens);
    CLASS_TO_INTERFACE(ComTokenSource,TokenSource,pWrap,pXThis);
    ITokenSource* pCom = NULL;
    pXThis->QueryInterface(FX_NS(plugin)::ITokenSource::iid,(void**)&pCom);
    return pCom;
}

FX_NS(analyzer)::TokenSource* ComTokenSource::unwrap(ITokenSource* pInterface)
{
    if ( pInterface == NULL ) return NULL;
    INTERFACE_TO_CLASS(ComTokenSource, TokenSource, pInterface, pWrapper)
    return (FX_NS(analyzer)::TokenSource*)pWrapper->m_pTokens;
}

ITokenSource* ComTokenSource::getInterface()
{
    CLASS_TO_INTERFACE(ComTokenSource, TokenSource, this, pXThis);
    ITokenSource* pCom = NULL;
    pXThis->QueryInterface(ITokenSource::iid, (void**)&pCom);
    return pCom;
}

//////////////////////////////////////////////////////////////////////////
//ComReader ITokenSource
IMPLEMENT_UNKNOWN(ComTokenSource, TokenSource)

FX_STDMETHODIMP ComTokenSource::XTokenSource::acquireCustomView(
        /* [in] */ FX_NS(com)::FX_BSTR viewName, 
        /* [out][retval */ ITokenView** ppTokenView)
{
    FX_METHOD_PROLOGUE( ComTokenSource, TokenSource);
    
    ComStr comStrName(viewName);
    tstring sName;
    comStrName.copyTo(sName);
    
    TokenViewPtr pTokenView = pThis->m_pTokens->acquireCustomView(sName);
    *ppTokenView = ComTokenView::wrap(const_cast<TokenView*>(pTokenView.get()));

    return FX_S_OK;
}

FX_STDMETHODIMP ComTokenSource::XTokenSource::getOriginalView(
        /* [out][retval */ ITokenView** ppTokenView)
{
    FX_METHOD_PROLOGUE(ComTokenSource, TokenSource);
    
    TokenViewPtr pTokenView = pThis->m_pTokens->getOriginalView();
    *ppTokenView = ComTokenView::wrap(const_cast<TokenView*>(pTokenView.get()));

    return FX_S_OK;
}
FX_STDMETHODIMP ComTokenSource::XTokenSource::getLastView(
        /* [out][retval */ ITokenView** ppTokenView)
{
    FX_METHOD_PROLOGUE(ComTokenSource, TokenSource);
    
    TokenViewPtr pTokenView = pThis->m_pTokens->getLastView();
    *ppTokenView = ComTokenView::wrap(const_cast<TokenView*>(pTokenView.get()));

    return FX_S_OK;
}

FX_STDMETHODIMP ComTokenSource::XTokenSource::reset()
{
    FX_METHOD_PROLOGUE( ComTokenSource, TokenSource);
    pThis->m_pTokens->reset();

    return FX_S_OK;
}

FX_NS_END

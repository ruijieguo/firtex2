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
// Created	: 2006/5/15
//
#include "firtex/extension/plugin/ComDocument.h"

#include "firtex/extension/plugin/ComDocumentSchema.h"
#include "firtex/extension/com/ComPtr.h"

FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(com);

FX_NS_DEF(plugin)

ComDocument::ComDocument(Document* pDoc)
: m_pDoc(pDoc)
{
}

ComDocument::~ComDocument(void)
{
    m_pDoc = NULL;
}
IDocument* ComDocument::wrap(Document* pDoc)
{
    ComDocument* pWrap = new ComDocument(pDoc);
    CLASS_TO_INTERFACE(ComDocument, Document, pWrap, pXThis);
    IDocument* pCom = NULL;
    pXThis->QueryInterface(IDocument::iid, (void**)&pCom);
    return pCom;
}
Document* ComDocument::unwrap(IDocument* pInterface)
{
    if ( pInterface == NULL ) return NULL;
    INTERFACE_TO_CLASS(ComDocument, Document, pInterface, pWrapper)
    return (Document*)pWrapper->m_pDoc;
}

IDocument* ComDocument::getInterface()
{
    CLASS_TO_INTERFACE(ComDocument, Document, this, pXThis);
    IDocument* pCom = NULL;
    pXThis->QueryInterface(IDocument::iid, (void**)&pCom);
    return pCom;
}
//////////////////////////////////////////////////////////////////////////
//ComDocument IDocument
IMPLEMENT_UNKNOWN(ComDocument, Document)

FX_STDMETHODIMP ComDocument::XDocument::addField(
        /* [in] */ FX_NS(com)::FX_INT32 fieldId,
        /* [in] */ FX_NS(com)::FX_VARIANT* pVar,
        /* [in] */ FX_NS(com)::FX_BOOL bNoCopy)
{
    FX_METHOD_PROLOGUE(ComDocument, Document);

    if (pVar->vt != FX_VT_STRING)
    {
        return FX_S_FALSE;
    }

    pThis->m_pDoc->addField((fieldid_t)fieldId, pVar->sVal.str,
                            pVar->sVal.length, (bNoCopy != 0));
    return FX_S_OK;
}

FX_STDMETHODIMP ComDocument::XDocument::setBoost (/* [in] */ FX_NS(com)::FX_FLOAT boost)
{
    FX_METHOD_PROLOGUE(ComDocument, Document);
    pThis->m_pDoc->setBoost((float)boost);
    return FX_S_OK;
}

FX_NS_END


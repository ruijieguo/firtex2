//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2006/7/2
//

#include "firtex/extension/plugin/ComDocumentSchema.h"
#include "firtex/extension/com/ComPtr.h"

FX_NS_USE(document);
FX_NS_USE(com);
FX_NS_DEF(plugin);

ComDocumentSchema::ComDocumentSchema(DocumentSchema* pDocSchema)
    : m_pDocSchema(pDocSchema)
{
}

ComDocumentSchema::~ComDocumentSchema(void)
{
    m_pDocSchema = NULL;
}

IDocumentSchema* ComDocumentSchema::wrap(DocumentSchema* pDocSchema)
{
    ComDocumentSchema* pWrap = new ComDocumentSchema(pDocSchema);
    CLASS_TO_INTERFACE(ComDocumentSchema,DocumentSchema,pWrap,pXThis);
    IDocumentSchema* pCom = NULL;
    pXThis->QueryInterface(IDocumentSchema::iid,(void**)&pCom);
    return pCom;
}

DocumentSchema* ComDocumentSchema::unwrap(IDocumentSchema* pInterface)
{
    if ( pInterface == NULL ) return NULL;
    INTERFACE_TO_CLASS(ComDocumentSchema, DocumentSchema, pInterface, pWrapper)
    return (DocumentSchema*)pWrapper->m_pDocSchema;
}

IDocumentSchema* ComDocumentSchema::getInterface()
{
    CLASS_TO_INTERFACE(ComDocumentSchema, DocumentSchema, this, pXThis);
    IDocumentSchema* pCom = NULL;
    pXThis->QueryInterface(IDocumentSchema::iid, (void**)&pCom);
    return pCom;
}

//////////////////////////////////////////////////////////////////////////
//ComDocumentSchema IDocument
IMPLEMENT_UNKNOWN(ComDocumentSchema,DocumentSchema)

FX_STDMETHODIMP ComDocumentSchema::XDocumentSchema::addField(
        /* [in] */ FX_NS(com)::FX_BSTR name, 
        /* [in] */ FX_NS(com)::FX_BSTR type, 
        /* [in] */ FX_NS(com)::FX_BOOL isStore,
        /* [in] */ FX_NS(com)::FX_FLOAT boost,
        /* [out] */ FX_NS(com)::FX_INT32* pFieldId)
{
    ComStr comStrName(name);
    tstring sName;
    comStrName.copyTo(sName);

    ComStr comStrType(type);
    tstring sType;
    comStrType.copyTo(sType);

    FX_METHOD_PROLOGUE(ComDocumentSchema, DocumentSchema);
    *pFieldId = pThis->m_pDocSchema->addField(sName, sType,
            (isStore == 0) ? false : true, 
            false, boost);

    return FX_S_OK;
}

FX_STDMETHODIMP ComDocumentSchema::XDocumentSchema::getFieldId(
        /* [in] */ FX_NS(com)::FX_BSTR name,
        /* [out] */ FX_NS(com)::FX_INT32* pFieldId)
{
    ComStr comStrName(name);
    tstring sName;
    comStrName.copyTo(sName);

    FX_METHOD_PROLOGUE(ComDocumentSchema, DocumentSchema);
    *pFieldId = pThis->m_pDocSchema->getFieldId(sName);
    return FX_S_OK;
}

FX_NS_END

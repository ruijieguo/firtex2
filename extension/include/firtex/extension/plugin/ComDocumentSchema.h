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
#ifndef __FX_COMDOCUMENTSCHEMA_H
#define __FX_COMDOCUMENTSCHEMA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/extension/plugin/FirteXCOM.h"
#include "firtex/extension/plugin/ComObject.h"
#include "firtex/document/DocumentSchema.h"

FX_NS_DEF(plugin);

class ComDocumentSchema : public ComObject
{
public:
    ComDocumentSchema(FX_NS(document)::DocumentSchema* pDocSchema);
    virtual ~ComDocumentSchema(void);

public:
    static IDocumentSchema* wrap(FX_NS(document)::DocumentSchema* pDocSchema);
    static FX_NS(document)::DocumentSchema* unwrap(IDocumentSchema* pInterface);

public:
    IDocumentSchema* getInterface();

public:
    BEGIN_INTERFACE_PART(DocumentSchema, IDocumentSchema)

    FX_STDMETHOD(addField)(/* [in] */ FX_NS(com)::FX_BSTR name, 
                           /* [in] */ FX_NS(com)::FX_BSTR type, 
                           /* [in] */ FX_NS(com)::FX_BOOL isStore,
                           /* [in] */ FX_NS(com)::FX_FLOAT boost,
                           /* [out] */ FX_NS(com)::FX_INT32* pFieldId);

    FX_STDMETHOD(getFieldId)(/* [in] */ FX_NS(com)::FX_BSTR name,
                             /* [out] */ FX_NS(com)::FX_INT32* pFieldId);

    END_INTERFACE_PART(DocumentSchema);

private:
    FX_NS(document)::DocumentSchema* m_pDocSchema;
};

FX_NS_END

#endif

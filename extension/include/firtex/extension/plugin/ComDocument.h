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
// Created	: 2006/5/15
//
#ifndef __FX_COMDOCUMENT_H
#define __FX_COMDOCUMENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FirteXCOM.h"
#include "ComObject.h"
#include "firtex/document/Document.h"

FX_NS_DEF(plugin);

class ComDocument : public ComObject
{
public:
    ComDocument(FX_NS(document)::Document* pDoc);
    virtual ~ComDocument(void);

public:
    static IDocument* wrap(FX_NS(document)::Document* pDoc);
    static FX_NS(document)::Document* unwrap(IDocument* pInterface);

public:
    IDocument* getInterface();

public:
    BEGIN_INTERFACE_PART(Document, IDocument)

    FX_STDMETHOD(addField)(/* [in] */ FX_NS(com)::FX_INT32 fieldId,
                           /* [in] */ FX_NS(com)::FX_VARIANT* pVar,
                           /* [in] */ FX_NS(com)::FX_BOOL bNoCopy);

    FX_STDMETHOD(setBoost)(/* [in] */ FX_NS(com)::FX_FLOAT boost);

    END_INTERFACE_PART(Document);

protected:
    FX_NS(document)::Document* m_pDoc;
};

FX_NS_END

#endif

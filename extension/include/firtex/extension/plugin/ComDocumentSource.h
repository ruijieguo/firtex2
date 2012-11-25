//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-07-09 00:20:08

#ifndef __FX_COMDOCUMENTSOURCE_H
#define __FX_COMDOCUMENTSOURCE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/plugin/ComObject.h"
#include "firtex/extension/plugin/FirteXCOM.h"
#include "firtex/extension/processor/DocumentSource.h"

FX_NS_DEF(plugin);

class ComDocumentSource : public ComObject
{
public:
    ComDocumentSource(FX_NS(processor)::DocumentSource* pDocSrc);
    virtual ~ComDocumentSource();

public:
    static IDocumentSource* wrap(FX_NS(processor)::DocumentSource* pDoc);
    static FX_NS(processor)::DocumentSource* unwrap(IDocumentSource* pInterface);

public:
    IDocumentSource* getInterface();

public:
    BEGIN_INTERFACE_PART(DocumentSource, IDocumentSource)

    FX_STDMETHOD(setRawDocument)(/* [in] */ IRawDocument* pRawDoc);
    FX_STDMETHOD(getRawDocument)(/* [out] */ IRawDocument** ppRawDoc);
    FX_STDMETHOD(acquireDocument)(/* [in] */ FX_NS(com)::FX_BSTR processorName,
                                  /* [out] */ IDocument** ppDocument);
    FX_STDMETHOD(stealLastDocument)(/* [out] */ IDocument** ppDocument);
    FX_STDMETHOD(toBeContinued)(/* [out] */ FX_NS(com)::FX_BOOL* bContinue);
    FX_STDMETHOD(setToBeContinued)(/* [in] */ FX_NS(com)::FX_BOOL bContinue);
    FX_STDMETHOD(reset)();

    END_INTERFACE_PART(DocumentSource);

private:
    FX_NS(processor)::DocumentSource* m_pDocSrc;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_COMDOCUMENTSOURCE_H

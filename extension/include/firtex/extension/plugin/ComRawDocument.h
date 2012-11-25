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
// Created : 2011-07-09 09:45:36

#ifndef __FX_COMRAWDOCUMENT_H
#define __FX_COMRAWDOCUMENT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/plugin/FirteXCOM.h"
#include "firtex/extension/plugin/ComObject.h"
#include "firtex/extension/processor/RawDocument.h"

FX_NS_DEF(plugin);

class ComRawDocument : public ComObject
{
public:
    ComRawDocument(FX_NS(processor)::RawDocument* pRawDoc);
    ~ComRawDocument();

public:
    static IRawDocument* wrap(FX_NS(processor)::RawDocument* pDocSrc);
    static FX_NS(processor)::RawDocument* unwrap(IRawDocument* pInterface);
    IRawDocument* getInterface();

public:
    BEGIN_INTERFACE_PART(RawDocument, IRawDocument)

    FX_STDMETHOD(isPath)(/* [out] */ FX_NS(com)::FX_BOOL* pPath);
    FX_STDMETHOD(isContent)(/* [out] */ FX_NS(com)::FX_BOOL* pContent);
    FX_STDMETHOD(getPath)(/* [out] */ FX_NS(com)::FX_VARIANT** ppPath);
    FX_STDMETHOD(setPath)(/* [in] */ FX_NS(com)::FX_VARIANT* pPath);
    FX_STDMETHOD(getContent)(/* [out] */ FX_NS(com)::FX_VARIANT** pContent);
    FX_STDMETHOD(setContent)(/* [in] */ FX_NS(com)::FX_VARIANT* pContent);
    FX_STDMETHOD(clear)();

    END_INTERFACE_PART(RawDocument);

private:
    FX_NS(processor)::RawDocument* m_pRawDoc;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_COMRAWDOCUMENT_H

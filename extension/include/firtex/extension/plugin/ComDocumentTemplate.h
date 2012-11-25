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
// Created : 2011-07-09 00:26:07

#ifndef __FX_COMDOCUMENTTEMPLATE_H
#define __FX_COMDOCUMENTTEMPLATE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/plugin/FirteXCOM.h"
#include "firtex/extension/plugin/ComObject.h"
#include "firtex/extension/processor/DocumentTemplate.h"

FX_NS_DEF(plugin);

class ComDocumentTemplate : public ComObject
{
public:
    ComDocumentTemplate(FX_NS(processor)::DocumentTemplate* pTemp);
    ~ComDocumentTemplate();

public:
    static IDocumentTemplate* wrap(FX_NS(processor)::DocumentTemplate* pTemp);
    static FX_NS(processor)::DocumentTemplate* unwrap(IDocumentTemplate* pInterface);
    IDocumentTemplate* getInterface();

public:
    BEGIN_INTERFACE_PART(DocumentTemplate, IDocumentTemplate)

    FX_STDMETHOD(getDocTag)(/* [out] */ FX_NS(com)::FX_VARIANT** ppDocTag);
    FX_STDMETHOD(getTagCount)(/* [out] */ FX_NS(com)::FX_INT32* pTagCount);
    FX_STDMETHOD(getEmbeddedField)(/* [out] */ FX_NS(com)::FX_VARIANT** ppField);
    FX_STDMETHOD(getTag)(/* [in] */ FX_NS(com)::FX_INT32 tagIdx, 
                         /* [out] */ IFieldTag** ppTag);
    END_INTERFACE_PART(DocumentTemplate);

private:
    FX_NS(processor)::DocumentTemplate* m_pTemp;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_COMDOCUMENTTEMPLATE_H

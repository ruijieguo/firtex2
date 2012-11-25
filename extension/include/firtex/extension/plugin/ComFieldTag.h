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
// Created : 2011-07-09 11:44:48

#ifndef __FX_COMFIELDTAG_H
#define __FX_COMFIELDTAG_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/plugin/ComObject.h"
#include "firtex/extension/plugin/FirteXCOM.h"
#include "firtex/extension/processor/DocumentTemplate.h"

FX_NS_DEF(plugin);

class ComFieldTag : public ComObject
{
    typedef FX_NS(processor)::DocumentTemplate::Tag FieldTag;
    
public:
    ComFieldTag(FieldTag* pTag);
    ~ComFieldTag();

public:
    static IFieldTag* wrap(FieldTag* pTag);
    static FieldTag* unwrap(IFieldTag* pInterface);
    IFieldTag* getInterface();

public:
    BEGIN_INTERFACE_PART(FieldTag, IFieldTag)

    FX_STDMETHOD(getId)(/* [out] */ FX_NS(com)::FX_INT32* pTagId);
    FX_STDMETHOD(getTagName)(/* [out] */ FX_NS(com)::FX_VARIANT** ppTagName);
    FX_STDMETHOD(getAlias)(/* [out] */ FX_NS(com)::FX_VARIANT** ppTagAlias);
    FX_STDMETHOD(getFieldName)(/* [out] */ FX_NS(com)::FX_VARIANT** ppFieldName);

    END_INTERFACE_PART(FieldTag);

private:
    FieldTag* m_pTag;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_COMFIELDTAG_H

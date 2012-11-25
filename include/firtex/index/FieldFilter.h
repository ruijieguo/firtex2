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
// Created : 2011-10-17 22:53:03

#ifndef __FX_FIELDFILTER_H
#define __FX_FIELDFILTER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

class FieldFilter
{
public:
    FieldFilter() {}
    virtual ~FieldFilter() {}

public:
    /**
     * Make some modification on sOrgValue and output to sNewValue
     *
     * @param sOrgValue original value 
     * @param sNewValue the output value
     * 
     * @return true if any modification performed on sOrgValue
     */
    virtual bool filter(const std::string& sOrgValue, 
                        std::string& sNewValue) const = 0;
};

DEFINE_TYPED_PTR(FieldFilter);

FX_NS_END

#endif //__FX_FIELDFILTER_H

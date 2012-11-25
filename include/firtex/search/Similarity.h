//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-11-12 22:48:49

#ifndef __FX_SIMILARITY_H
#define __FX_SIMILARITY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(search);

class Similarity
{
public:
    Similarity();
    virtual ~Similarity();

public:

protected:

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_SIMILARITY_H

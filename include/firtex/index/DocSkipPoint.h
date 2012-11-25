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
// Created	: 2010-08-15 14:10:02

#ifndef __FX_DOCSKIPPOINT_H
#define __FX_DOCSKIPPOINT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

struct DocSkipPoint
{
    DocSkipPoint()
        : docId(0), cumSumTf(0), docOff(0)
    {
    }

    DocSkipPoint(docid_t d, ctf_t c, offset_t o)
        : docId(d), cumSumTf(c), docOff(o)
    {
    }

    DocSkipPoint& operator -= (const DocSkipPoint& other)
    {
        docId -= other.docId;
        cumSumTf -= other.cumSumTf;
        docOff -= other.docOff;
        return *this;
    }

    docid_t docId;
    ctf_t cumSumTf; //Cumulated sum of tf in skip block
    offset_t docOff;   //Offset into doc-list stream
};

FX_NS_END

#endif //__FX_DOCSKIPPOINT_H

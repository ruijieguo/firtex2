//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2006/12/02
//
#ifndef _IN_MEMORYTERMREADER_H
#define _IN_MEMORYTERMREADER_H

#include "TermReader.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(index)
class PostingWriter;
class FieldInfo;
class InMemoryTermReader : public TermReader
{
public:
    InMemoryTermReader()
    {
    }

    virtual ~InMemoryTermReader(void)
    {
    }

public:

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif

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
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2005/12/16
//
#include "firtex/utility/Exception.h"
#include "firtex/utility/NumberFormatter.h"

FX_NS_DEF(utility)

FirteXException::FirteXException()
: m_sWhat( _T("FirteX exception") )
{
}

FirteXException::FirteXException(const tstring& msg)
    : m_sWhat(msg)
{
}

//! Create a new exception
FirteXException::FirteXException(const tstring& msg, const tstring& file,int line)		
{
    tstring sMsg = msg;
    sMsg += _T(": at file: ");
    sMsg += file;
    sMsg += _T(", line: ");
    NumberFormatter::append(sMsg, (int32_t)line);

    m_sWhat = sMsg;
}

FX_NS_END


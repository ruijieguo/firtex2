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
// Created	: 2006/5/20
//
#include <string>

#ifndef HAVE_TCHAR_H
#if defined(_UCS2) || defined(_UNICODE)

#undef _T //remove any previously defined _T - required for ppc os
#define _T(x)       L ## x

#define tchar wchar_t
#define tstring	std::wstring

#define tstringstream std::wstringstream
	
#else //if defined(_ASCII)

#undef _T //remove any previously defined _T - required for ppc os
#define _T(x)      x

#define tchar char
#define tstring std::string

#define tstringstream std::stringstream

#endif

#else //HAVE_TCHAR_H

#include <tchar.h>

#if defined(_UCS2) || defined(_UNICODE)
#define tstring	std::wstring
#define tchar wchar_t
	
#else 
#define tstring std::string
#define tchar char
	    
#endif
	
#endif

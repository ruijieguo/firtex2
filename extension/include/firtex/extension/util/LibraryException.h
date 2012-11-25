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
// Email	: ruijieguo@gmail.com
// Created	: 2010/2/13
//
#ifndef ____LIBRARYEXCEPTION_H__
#define ____LIBRARYEXCEPTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/utility/Exception.h"

FX_NS_DEF(util)

/**
 * @class LibraryLoadException
 *
 * Used to describe a runtime exception
 */
class LibraryLoadException : public FX_NS(utility)::FirteXException
{
public:
	// Create a new exception
	LibraryLoadException () : FirteXException( _T("Library load exception") ){}

	// Create a new exception
	LibraryLoadException (const tstring& msg) : FirteXException(msg) { }

	// Create a new exception
	LibraryLoadException (const tstring& prefix, const tstring& msg) 
        : FirteXException(prefix + msg) { }
};

/**
 * @class LibraryAlreadyLoadedException
 *
 * Used to describe a runtime exception
 */
class LibraryAlreadyLoadedException : public FX_NS(utility)::FirteXException
{
public:
	// Create a new exception
	LibraryAlreadyLoadedException () : FirteXException( _T("Library already loaded.") ){}
	// Create a new exception
	LibraryAlreadyLoadedException (const tstring& msg) 
        : FirteXException( tstring(_T("Library already loaded: ")) + msg) { }
};

FX_NS_END
#endif



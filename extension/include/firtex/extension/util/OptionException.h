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
// Created	: 2009/6/10
//
#ifndef ____OPTIONEXCEPTION_H__
#define ____OPTIONEXCEPTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/utility/Exception.h"

FX_NS_DEF(util)		
/**
 * @class OptionException
 *
 * Used to describe a runtime exception
 */
class OptionException : public FX_NS(utility)::FirteXException
{
public:
    // Create a new exception
    OptionException () : FirteXException( _T("Option exception") ){}

    // Create a new exception
    OptionException (const tstring& msg) : FirteXException(msg) { }

    // Create a new exception
    OptionException (const tstring& prefix, const tstring& msg) 
        : FirteXException(prefix + msg) { }
};

/**
 * @class DuplicateOptionException
 *
 * Used to describe a runtime exception
 */
class DuplicateOptionException : public OptionException
{
public:
    // Create a new exception
    DuplicateOptionException () : OptionException( _T("Option duplicated") ){}
    // Create a new exception
    DuplicateOptionException (const tstring& msg) 
        : OptionException( _T("Option duplicated: "), msg) { }
};

/**
 * @class UnknownOptionException
 *
 * Used to describe a runtime exception
 */
class UnknownOptionException : public OptionException
{
public:
    // Create a new exception
    UnknownOptionException () : OptionException( _T("Unknown option.") ){}

    // Create a new exception
    UnknownOptionException (const tstring& msg) 
        : OptionException(_T("Unknown option: "), msg) { }
};

/**
 * @class UnexpectedArgumentException
 *
 * Used to describe a runtime exception
 */
class UnexpectedArgumentException : public OptionException
{
public:
    // Create a new exception
    UnexpectedArgumentException () 
        : OptionException( _T("Argument is unexpected.") ){}

    // Create a new exception
    UnexpectedArgumentException (const tstring& msg) 
        : OptionException(_T("Argument is unexpected: "), msg) { }
};

FX_NS_END

#endif



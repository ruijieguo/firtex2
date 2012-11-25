//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2009/6/10
//
#ifndef __FX_NETWORKEXCEPTION_H__
#define __FX_NETWORKEXCEPTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/utility/Exception.h"

FX_NS_DEF(utility);

/**
 * @class NetworkException
 *
 * Used to describe a runtime exception
 */
class NetworkException : public FirteXException
{
public:
    // Create a new exception
    NetworkException() : FirteXException( _T("Network exception") ){}

    // Create a new exception
    NetworkException(const tstring& msg) : FirteXException(msg) { }

    // Create a new exception
    NetworkException(const tstring& msg, const tstring& file, int line)
        : FirteXException(msg, file, line) { }
};

#define FIRTEX_DECLARE_NET_EXCEPTION(exp_class)                         \
    class exp_class : public NetworkException                           \
    {                                                                   \
    public:                                                             \
        exp_class() : NetworkException(_T(#exp_class) ){}               \
        exp_class(const tstring& msg) : NetworkException(msg) { }       \
        exp_class(const tstring& msg, const tstring& file, int line)    \
            : NetworkException(msg, file, line) {}                      \
        exp_class(const tstring& prefix, const tstring& msg)            \
            : NetworkException(prefix + msg) { }                        \
    };


/**
 * @class HostNotFoundException
 *
 * Used to describe a host not found exception
 */
FIRTEX_DECLARE_NET_EXCEPTION(HostNotFoundException);

/**
 * @class ServiceNotFoundException
 *
 * Used to describe a host not found exception
 */
FIRTEX_DECLARE_NET_EXCEPTION(ServiceNotFoundException);

/**
 * @class ConnectionAbortedException
 *
 * Used to describe a host not found exception
 */
FIRTEX_DECLARE_NET_EXCEPTION(ConnectionAbortedException);

/**
 * @class ConnectionResetException
 *
 * Used to describe a host not found exception
 */
FIRTEX_DECLARE_NET_EXCEPTION(ConnectionResetException);

/**
 * @class ConnectionRefusedException
 *
 * Used to describe a host not found exception
 */
FIRTEX_DECLARE_NET_EXCEPTION(ConnectionRefusedException);

/**
 * @class InvalidAddressException
 *
 * Used to describe a host not found exception
 */
FIRTEX_DECLARE_NET_EXCEPTION(InvalidAddressException);

/**
 * @class NoAddressFoundException
 *
 * Used to describe a host not found exception
 */
FIRTEX_DECLARE_NET_EXCEPTION(NoAddressFoundException);

/**
 * @class DNSException
 *
 * Used to describe a host not found exception
 */
FIRTEX_DECLARE_NET_EXCEPTION(DNSException);

FX_NS_END

#endif



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
// Created : 2011-05-07 15:35:58

#ifndef __FX_ICONVWRAPPER_H
#define __FX_ICONVWRAPPER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include <string>

FX_NS_DEF(utility);

class IconvWrapper
{
public:
    typedef void* iconv_t;

public:
    IconvWrapper();
    ~IconvWrapper();

public:
    /**
     * Init resource for encoding convertion from /sFromCode/ to /SToCode/
     * @param sToCode target encode name
     * @param sFromCode source encode name
     * @throws throw BadParameterException if failure.
     */
    void open(const std::string& sToCode,
              const std::string& sFromCode);

    /**
     * Convert /sInStr/ to /sOutStr/
     *
     * @param sOutStr store convertion result
     * @param sInStr source string
     *
     * @return true if convert success
     */
    bool convert(std::string& sOutStr, const std::string& sInStr) const;

    /**
     * Convert string buffer
     *
     * @param szOutBuf store convertion result
     * @param nOutBufLen manx length of output buffer
     * @param szInBuf source string buffer
     * @param nInBufLen source buffer length
     *
     * @return converted bytes, return 0 if failure or no bytes to convert
     */
    size_t convert(char* szOutBuf, size_t nOutBufLen, 
                   const char* szInBuf, size_t nInBufLen) const;

    /**
     * Release resource
     */
    void close();

private:
    iconv_t m_handle;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(IconvWrapper);

FX_NS_END

#endif //__FX_ICONVWRAPPER_H

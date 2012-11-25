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
// Created	: 2010-08-10 22:56:32

#ifndef __FX_POSTINGCODING_H
#define __FX_POSTINGCODING_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/OutputStream.h"
#include "firtex/store/InputStream.h"

FX_NS_DEF(index);

class PostingCoding
{
public:
    PostingCoding() {}
    virtual ~PostingCoding() {}

public:
    /**
     * Encode posting(represented by int32) list to byte slice list
     * @param outStream slice list writer
     * @param pBuffer posting buffer
     * @param bSeal seal the last record if true
     * @param n number of element in buffer
     */
    virtual void encodeInt32(FX_NS(store)::OutputStream& outStream,
                             const int32_t* pBuffer, size_t n,
                             bool bSeal) const = 0;

    /**
     * Decode posting(represented by int32) list from byte slice list
     * @param inStream file input stream
     * @param pBuffer posting buffer
     * @param nBufSize size of buffer
     * @param bExhausted set true if encoded records are exhausted
     * @return number of decoded elements
     */
    virtual size_t decodeInt32(int32_t* pBuffer, size_t nBufSize,
                               FX_NS(store)::InputStream& inStream,
                               bool& bExhausted) const = 0;
};

FX_NS_END

#endif //__FX_POSTINGCODING_H

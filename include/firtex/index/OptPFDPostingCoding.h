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
// Created	: 2010-08-11 21:52:47

#ifndef __FX_OPTPFDPOSTINGCODING_H
#define __FX_OPTPFDPOSTINGCODING_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/PostingCoding.h"
#include "firtex/index/compress/OptPForDeltaCoding.h"

FX_NS_DEF(index);

class OptPFDPostingCoding : public PostingCoding
{
public:
    const static size_t CODING_BUFFER_SIZE = OptPForDeltaCoding::FRAME_SIZE + OptPForDeltaCoding::FRAME_SIZE/2;

public:
    OptPFDPostingCoding();
    ~OptPFDPostingCoding();

public:
    /**
     * Encode posting(represented by int32) list to byte slice list
     * @param outStream slice list writer
     * @param pBuffer posting buffer
     * @param n number of element in buffer
     * @param bSeal seal the last record if true
     */
    void encodeInt32(FX_NS(store)::OutputStream& outStream,
                     const int32_t* pBuffer, size_t n,
                     bool bSeal) const;

    /**
     * Decode posting(represented by int32) list from byte slice list
     * @param inStream file input stream
     * @param pBuffer posting buffer
     * @param nBufSize size of buffer
     * @param bExhausted set true if encoded records are exhausted
     * @return number of decoded elements
     */
    size_t decodeInt32(int32_t* pBuffer, size_t nBufSize,
                       FX_NS(store)::InputStream& inStream,
                       bool& bExhausted) const;
    
private:
    OptPForDeltaCoding m_coding;
};

/////////////////////////////////////////////////////////
//

FX_NS_END

#endif //__FX_OPTPFDPOSTINGCODING_H

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
// Created	: 2010-08-06 22:59:33

#ifndef __FX_OPTPFORDELTACODING_H
#define __FX_OPTPFORDELTACODING_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Exception.h"
#include "firtex/index/compress/Coding.h"
#include "firtex/index/compress/Pack.h"
#include "firtex/index/compress/Unpack.h"
#include "firtex/index/compress/S16Coding.h"

#ifndef FX_WINDOWS
#define USE_ASM_OPTIMIZE
#endif

FX_NS_DEF(index);

class OptPForDeltaCoding : public Coding
{
public:
    const static size_t FRAME_SIZE = 128;
    const static size_t HEADER_INT_SIZE = 1;

    const static uint32_t EXCEPTION_RATIO = 10; //exception rate: 10%

public:
    OptPForDeltaCoding();
    ~OptPForDeltaCoding();

public:
    /**
     * Encode uint32 array
     * @param dest bytes array to encode to
     * @param nDestLen length of dest
     * @param src source uint32 array to encode
     * @param nSrcLen length of source array
     * @param bSeal seal the last record if true
     * @return encoded length in uint32 size
     */
    size_t encode(uint32_t* dest, size_t nDestLen,
                  const uint32_t* src, size_t nSrcLen,
                  bool bSeal) const;

    /**
     * Decode uint32 array
     * @param dest bytes array to decode to
     * @param nDestLen length of dest
     * @param src source uint32 array to decode
     * @param nSrcLen length of source array
     * @param bExhausted set true if encoded records are exhausted
     * @return number of decoded uint32 elements
     */
    size_t decode(uint32_t* dest, size_t nDestLen,
                  const uint32_t* src, size_t nSrcLen,
                  bool& bExhausted) const;

public:
    /**
     * Return frame size by parsing header of frame
     */
    size_t getFrameSize(uint32_t header) const;

protected:
    inline uint32_t getHighBitOff(uint32_t value) const;
    inline void getHighBitArray(uint32_t* bitArray, uint32_t& maxBitOff,
                                const uint32_t* frame, size_t length) const;

    inline uint32_t determinFrameBits(const uint32_t* frame,
                                      uint32_t nFrameLen, uint32_t& nNumExceps) const;
    inline size_t doEncodeFrame(uint32_t* dest, size_t nDestLen, 
                                uint32_t nFrameBits, size_t nFirstExcepPos,
                                const uint32_t* src, size_t nSrcLen,
                                uint32_t* exception, uint32_t* excepOff,
                                size_t nNumExceps, bool bLastFrame) const;
    inline uint32_t encodeHeader(uint32_t nFrameBits, size_t nFirstExcepPos,
                                 size_t nSrcLen, uint32_t nNumExceps, 
                                 uint32_t nExcepIntSize, bool bLastFrame) const;
    inline size_t encodeException(uint32_t* dest, size_t nDestLen,
                                  uint32_t* exceptions, uint32_t* excepOffs,
                                  size_t nNumExceps) const;
    inline void decodeException(uint32_t* dest, uint32_t* src, size_t nSrcLen,
                                size_t nFirstExcepPos, uint32_t nFrameBits) const;

protected:
    size_t encodeFrame(uint32_t* dest, size_t nDestLen,
                       const uint32_t* src, size_t nSrcLen,
                       uint32_t nFrameBits, uint32_t nNumExceps,
                       bool bLastFrame) const;
    size_t decodeFrame(uint32_t* dest, size_t nDestLen, uint32_t*& src,
                       size_t& nSrcLen, bool& bLastFrame) const;

protected:
    uint32_t MASK[33];
    uint32_t BITS_PACK_MAP[33];

    S16Coding m_s16Coding;

private:
    DECLARE_STREAM_LOGGER();
};

//////////////////////////////////////////////////////////////////
//
inline uint32_t OptPForDeltaCoding::getHighBitOff(uint32_t value) const
{
#ifdef USE_ASM_OPTIMIZE
    int r = 0;

    __asm__("bsrl %1,%0\n\t"
            "cmovzl %2,%0"
            : "=&r" (r) : "rm" (value), "rm" (-1));
    return (uint32_t)(r + 1);
#else
    uint32_t i = 0;
    for (uint32_t b = value; b > 0; b >>= 1, ++i);
    return i;
#endif 

}

inline void OptPForDeltaCoding::getHighBitArray(uint32_t* bitArray,
        uint32_t& maxBitOff, const uint32_t* frame, size_t length) const
{
    size_t i;
    for (i = 32; i <= length; i += 32, frame += 32)
    {
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[0])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[1])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[2])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[3])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[4])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[5])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[6])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[7])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[8])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[9])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[10])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[11])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[12])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[13])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[14])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[15])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[16])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[17])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[18])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[19])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[20])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[21])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[22])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[23])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[24])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[25])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[26])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[27])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[28])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[29])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[30])]];
        ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[31])]];
    }

    if (length & 31)
    {
        for (i = 0; i < (length & 31); ++i)
        {
            ++bitArray[BITS_PACK_MAP[getHighBitOff(frame[i])]];
        }
    }

    int32_t j;
    for (j = 32; j >= 0 && bitArray[j] == 0; j--);
    maxBitOff= j;
}

inline uint32_t OptPForDeltaCoding::determinFrameBits(const uint32_t* frame,
        uint32_t nFrameLen, uint32_t& nNumExceps) const
{
    uint32_t bitArray[33] = {0};
    uint32_t maxBits = 0;    
    getHighBitArray(bitArray, maxBits, frame, nFrameLen);

    if (maxBits == 0)
    {
        nNumExceps = 0;
        return 0;
    }

    uint32_t exceptionCode = (maxBits < 8) ? 0 : (maxBits < 16) ? 1 : 2;
    uint32_t bytesPerException = (1 << exceptionCode);
    uint32_t nFrameBits = 0;
    uint32_t bytesForFrame = (uint32_t)(nFrameLen * nFrameBits + 7 ) / 8;

    // Assume all inputs are exceptions initially.
    uint32_t totalBytes = bytesForFrame + (uint32_t)nFrameLen * bytesPerException;
    uint32_t bestBytes = totalBytes;
    uint32_t bestFrameBits = nFrameBits;
    uint32_t totalExceptions = nFrameLen;
    uint32_t bestExceptions = nFrameLen;

    for (uint32_t i = 0; i <= maxBits; i++) 
    {
        if (bitArray[i] > 0)
        {
            nFrameBits = i;
            uint32_t newBytesForFrame = (nFrameLen * nFrameBits + 7 ) / 8;
            totalBytes += newBytesForFrame - bytesForFrame;
            bytesForFrame = newBytesForFrame;
            totalExceptions -= bitArray[i];
            totalBytes -= (bitArray[i] * bytesPerException);

            if (totalBytes <= bestBytes)
            {
                bestBytes = totalBytes;
                bestFrameBits = nFrameBits;
                bestExceptions = totalExceptions;
            }
        }
    }
    if (bestExceptions <= (uint32_t)((nFrameLen * EXCEPTION_RATIO)/100))
    {
        nNumExceps = bestExceptions;
        return bestFrameBits;
    }

    int32_t threshold = (int32_t)(((nFrameLen + 1) * (100 - EXCEPTION_RATIO)) / 100);
    bestExceptions = (uint32_t)(nFrameLen - (size_t)threshold);

    uint32_t i;
    for (i = 0; i <= maxBits && threshold > 0; ++i)
    {
        threshold -= bitArray[i];
    }
    nNumExceps = (bestExceptions + threshold);
    return (i - 1);
}

inline size_t OptPForDeltaCoding::doEncodeFrame(uint32_t* dest, size_t nDestLen,
        uint32_t nFrameBits, size_t nFirstExcepPos, const uint32_t* src, size_t nSrcLen,
        uint32_t* exception, uint32_t* excepOff, size_t nNumExceps, bool bLastFrame) const
{
    size_t excepOffIntSize = HEADER_INT_SIZE + (nSrcLen * nFrameBits + 31) / 32;
    if (nFirstExcepPos == 0)
    {
        FIRTEX_ASSERT2(nDestLen >= excepOffIntSize);

        memset(dest, 0, excepOffIntSize * sizeof(uint32_t));

        *dest++ = encodeHeader(nFrameBits, nFirstExcepPos, nSrcLen, 0, 0, bLastFrame);
        Pack::pack(nFrameBits, dest, src, nSrcLen);

        return excepOffIntSize;
    }

    //Encode exception
    size_t nExcepLen = encodeException(dest + excepOffIntSize, nDestLen - excepOffIntSize,
            exception, excepOff, nNumExceps);

    size_t intDataSize = excepOffIntSize + nExcepLen;
    FIRTEX_ASSERT2(nDestLen >= intDataSize);

    memset(dest, 0, excepOffIntSize * sizeof(uint32_t));

    // encode header
    *dest++ = encodeHeader(nFrameBits, nFirstExcepPos, nSrcLen, (uint32_t)nNumExceps, (uint32_t)nExcepLen, bLastFrame);

    // Pack normal data
    Pack::pack(nFrameBits, dest, src, nSrcLen);

    return intDataSize;
}

inline uint32_t OptPForDeltaCoding::encodeHeader(uint32_t nFrameBits, size_t nFirstExcepPos,
        size_t nSrcLen, uint32_t nNumExceps, uint32_t nExcepIntSize, bool bLastFrame) const
{
    // Header is encoded as follow:
    // 7bit : data length - 1
    // 8bit : first exception position
    // 5bit : frame bit
    // 11bit : exception byte range
    // 1bit : has next frame or not

    uint32_t len = (uint32_t)nSrcLen - 1;
    uint32_t lastBit = bLastFrame ? 1 : 0;
    if (nFrameBits > 31)
    {
        nFrameBits = 31;
    }
    return (len << 25 | nFirstExcepPos << 17
            | nFrameBits << 12 | nExcepIntSize << 1
            | lastBit);
}

inline size_t OptPForDeltaCoding::encodeException(uint32_t* dest, size_t nDestLen,
        uint32_t* exceptions, uint32_t* excepOffs, size_t nNumExceps) const
{
    uint32_t excepData[FRAME_SIZE];
    FIRTEX_ASSERT2(FRAME_SIZE >= nNumExceps * 2 - 1);

    for (size_t i = 0 ; i < nNumExceps; i++)
    {
        excepData[2 * i] = exceptions[i];
    }
    for (size_t i = 0; i < nNumExceps - 1; i++ )
    {
        excepData[2 * i + 1] = excepOffs[i];
    }

    //Encode exception value
    return m_s16Coding.doEncode(dest, nDestLen, excepData,
                                (nNumExceps << 1) - 1);
}

inline void OptPForDeltaCoding::decodeException(uint32_t* dest, uint32_t* src, size_t nSrcLen,
                                      size_t nFirstExcepPos, uint32_t nFrameBits) const
{
    size_t nCurOff = nFirstExcepPos;
    uint32_t adj = 0;
    for (size_t i = 0; i < nSrcLen; ++i, ++src)
    {
        uint32_t v = *src;
        uint32_t k = (v >> 28) + adj;
        switch(k)
        {
        case 0: // 28 numbers
            dest[nCurOff] |= ((v) & 1) << nFrameBits;
            nCurOff += ((v >> 1) & 1) + 1;
            dest[nCurOff] |= ((v >> 2) & 1) << nFrameBits;
            nCurOff += ((v >> 3) & 1) + 1;
            dest[nCurOff] |= ((v >> 4) & 1) << nFrameBits;
            nCurOff += ((v >> 5) & 1) + 1;
            dest[nCurOff] |= ((v >> 6) & 1) << nFrameBits;
            nCurOff += ((v >> 7) & 1) + 1;
            dest[nCurOff] |= ((v >> 8) & 1) << nFrameBits;
            nCurOff += ((v >> 9) & 1) + 1;
            dest[nCurOff] |= ((v >> 10) & 1) << nFrameBits;
            nCurOff += ((v >> 11) & 1) + 1;
            dest[nCurOff] |= ((v >> 12) & 1) << nFrameBits;
            nCurOff += ((v >> 13) & 1) + 1;
            dest[nCurOff] |= ((v >> 14) & 1) << nFrameBits;
            nCurOff += ((v >> 15) & 1) + 1;
            dest[nCurOff] |= ((v >> 16) & 1) << nFrameBits;
            nCurOff += ((v >> 17) & 1) + 1;
            dest[nCurOff] |= ((v >> 18) & 1) << nFrameBits;
            nCurOff += ((v >> 19) & 1) + 1;
            dest[nCurOff] |= ((v >> 20) & 1) << nFrameBits;
            nCurOff += ((v >> 21) & 1) + 1;
            dest[nCurOff] |= ((v >> 22) & 1) << nFrameBits;
            nCurOff += ((v >> 23) & 1) + 1;
            dest[nCurOff] |= ((v >> 24) & 1) << nFrameBits;
            nCurOff += ((v >> 25) & 1) + 1;
            dest[nCurOff] |= ((v >> 26) & 1) << nFrameBits;
            nCurOff += ((v >> 27) & 1) + 1;
            adj = 0;
            break;
        case 1: // 21 numbers
            dest[nCurOff] |= ((v) & 3) << nFrameBits;
            nCurOff += ((v >> 2) & 3) + 1;
            dest[nCurOff] |= ((v >> 4) & 3) << nFrameBits;
            nCurOff += ((v >> 6) & 3) + 1;
            dest[nCurOff] |= ((v >> 8) & 3) << nFrameBits;
            nCurOff += ((v >> 10) & 3) + 1;
            dest[nCurOff] |= ((v >> 12) & 3) << nFrameBits;
            nCurOff += ((v >> 14) & 1) + 1;
            dest[nCurOff] |= ((v >> 15) & 1) << nFrameBits;
            nCurOff += ((v >> 16) & 1) + 1;
            dest[nCurOff] |= ((v >> 17) & 1) << nFrameBits;
            nCurOff += ((v >> 18) & 1) + 1;
            dest[nCurOff] |= ((v >> 19) & 1) << nFrameBits;
            nCurOff += ((v >> 20) & 1) + 1;
            dest[nCurOff] |= ((v >> 21) & 1) << nFrameBits;
            nCurOff += ((v >> 22) & 1) + 1;
            dest[nCurOff] |= ((v >> 23) & 1) << nFrameBits;
            nCurOff += ((v >> 24) & 1) + 1;
            dest[nCurOff] |= ((v >> 25) & 1) << nFrameBits;
            nCurOff += ((v >> 26) & 1) + 1;
            dest[nCurOff] |= ((v >> 27) & 1) << nFrameBits;
            adj = 16;
            break;
        case 2: // 21 numbers
            dest[nCurOff] |= ((v) & 1) << nFrameBits;
            nCurOff += ((v >> 1) & 1) + 1;
            dest[nCurOff] |= ((v >> 2) & 1) << nFrameBits;
            nCurOff += ((v >> 3) & 1) + 1;
            dest[nCurOff] |= ((v >> 4) & 1) << nFrameBits;
            nCurOff += ((v >> 5) & 1) + 1;
            dest[nCurOff] |= ((v >> 6) & 1) << nFrameBits;
            nCurOff += ((v >> 7) & 3) + 1;
            dest[nCurOff] |= ((v >> 9) & 3) << nFrameBits;
            nCurOff += ((v >> 11) & 3) + 1;
            dest[nCurOff] |= ((v >> 13) & 3) << nFrameBits;
            nCurOff += ((v >> 15) & 3) + 1;
            dest[nCurOff] |= ((v >> 17) & 3) << nFrameBits;
            nCurOff += ((v >> 19) & 3) + 1;
            dest[nCurOff] |= ((v >> 21) & 1) << nFrameBits;
            nCurOff += ((v >> 22) & 1) + 1;
            dest[nCurOff] |= ((v >> 23) & 1) << nFrameBits;
            nCurOff += ((v >> 24) & 1) + 1;
            dest[nCurOff] |= ((v >> 25) & 1) << nFrameBits;
            nCurOff += ((v >> 26) & 1) + 1;
            dest[nCurOff] |= ((v >> 27) & 1) << nFrameBits;
            adj = 16;
            break;
        case 3:
            dest[nCurOff] |= ((v) & 1) << nFrameBits;
            nCurOff += ((v >> 1) & 1) + 1;
            dest[nCurOff] |= ((v >> 2) & 1) << nFrameBits;
            nCurOff += ((v >> 3) & 1) + 1;
            dest[nCurOff] |= ((v >> 4) & 1) << nFrameBits;
            nCurOff += ((v >> 5) & 1) + 1;
            dest[nCurOff] |= ((v >> 6) & 1) << nFrameBits;
            nCurOff += ((v >> 7) & 1) + 1;
            dest[nCurOff] |= ((v >> 8) & 1) << nFrameBits;
            nCurOff += ((v >> 9) & 1) + 1;
            dest[nCurOff] |= ((v >> 10) & 1) << nFrameBits;
            nCurOff += ((v >> 11) & 1) + 1;
            dest[nCurOff] |= ((v >> 12) & 1) << nFrameBits;
            nCurOff += ((v >> 13) & 1) + 1;
            dest[nCurOff] |= ((v >> 14) & 3) << nFrameBits;
            nCurOff += ((v >> 16) & 3) + 1;
            dest[nCurOff] |= ((v >> 18) & 3) << nFrameBits;
            nCurOff += ((v >> 20) & 3) + 1;
            dest[nCurOff] |= ((v >> 22) & 3) << nFrameBits;
            nCurOff += ((v >> 24) & 3) + 1;
            dest[nCurOff] |= ((v >> 26) & 3) << nFrameBits;
            adj = 16;
            break;
        case 4:
            dest[nCurOff] |= ((v) & 3) << nFrameBits;
            nCurOff += ((v >> 2) & 3) + 1;
            dest[nCurOff] |= ((v >> 4) & 3) << nFrameBits;
            nCurOff += ((v >> 6) & 3) + 1;
            dest[nCurOff] |= ((v >> 8) & 3) << nFrameBits;
            nCurOff += ((v >> 10) & 3) + 1;
            dest[nCurOff] |= ((v >> 12) & 3) << nFrameBits;
            nCurOff += ((v >> 14) & 3) + 1;
            dest[nCurOff] |= ((v >> 16) & 3) << nFrameBits;
            nCurOff += ((v >> 18) & 3) + 1;
            dest[nCurOff] |= ((v >> 20) & 3) << nFrameBits;
            nCurOff += ((v >> 22) & 3) + 1;
            dest[nCurOff] |= ((v >> 24) & 3) << nFrameBits;
            nCurOff += ((v >> 26) & 3) + 1;
            adj = 0;
            break;
        case 5:
            dest[nCurOff] |= ((v) & 15) << nFrameBits;
            nCurOff += ((v >> 4) & 7) + 1;
            dest[nCurOff] |= ((v >> 7) & 7) << nFrameBits;
            nCurOff += ((v >> 10) & 7) + 1;
            dest[nCurOff] |= ((v >> 13) & 7) << nFrameBits;
            nCurOff += ((v >> 16) & 7) + 1;
            dest[nCurOff] |= ((v >> 19) & 7) << nFrameBits;
            nCurOff += ((v >> 22) & 7) + 1;
            dest[nCurOff] |= ((v >> 25) & 7) << nFrameBits;
            adj = 16;
            break;
        case 6:
            dest[nCurOff] |= ((v) & 7) << nFrameBits;
            nCurOff += ((v >> 3) & 15) + 1;
            dest[nCurOff] |= ((v >> 7) & 15) << nFrameBits;
            nCurOff += ((v >> 11) & 15) + 1;
            dest[nCurOff] |= ((v >> 15) & 15) << nFrameBits;
            nCurOff += ((v >> 19) & 7) + 1;
            dest[nCurOff] |= ((v >> 22) & 7) << nFrameBits;
            nCurOff += ((v >> 25) & 7) + 1;
            adj = 0;
            break;
        case 7:
            dest[nCurOff] |= ((v) & 15) << nFrameBits;
            nCurOff += ((v >> 4) & 15) + 1;
            dest[nCurOff] |= ((v >> 8) & 15) << nFrameBits;
            nCurOff += ((v >> 12) & 15) + 1;
            dest[nCurOff] |= ((v >> 16) & 15) << nFrameBits;
            nCurOff += ((v >> 20) & 15) + 1;
            dest[nCurOff] |= ((v >> 24) & 15) << nFrameBits;
            adj = 16;
            break;
        case 8:
            dest[nCurOff] |= ((v) & 31) << nFrameBits;
            nCurOff += ((v >> 5) & 31) + 1;
            dest[nCurOff] |= ((v >> 10) & 31) << nFrameBits;
            nCurOff += ((v >> 15) & 31) + 1;
            dest[nCurOff] |= ((v >> 20) & 15) << nFrameBits;
            nCurOff += ((v >> 24) & 15) + 1;
            adj = 0;
            break;
        case 9:
            dest[nCurOff] |= ((v) & 15) << nFrameBits;
            nCurOff += ((v >> 4) & 15) + 1;
            dest[nCurOff] |= ((v >> 8) & 31) << nFrameBits;
            nCurOff += ((v >> 13) & 31) + 1;
            dest[nCurOff] |= ((v >> 18) & 31) << nFrameBits;
            nCurOff += ((v >> 23) & 31) + 1;
            adj = 0;
            break;
        case 10:
            dest[nCurOff] |= ((v) & 63) << nFrameBits;
            nCurOff += ((v >> 6) & 63) + 1;
            dest[nCurOff] |= ((v >> 12) & 63) << nFrameBits;
            nCurOff += ((v >> 18) & 31) + 1;
            dest[nCurOff] |= ((v >> 23) & 31) << nFrameBits;
            adj = 16;
            break;
        case 11:
            dest[nCurOff] |= ((v) & 31) << nFrameBits;
            nCurOff += ((v >> 5) & 31) + 1;
            dest[nCurOff] |= ((v >> 10) & 63) << nFrameBits;
            nCurOff += ((v >> 16) & 63) + 1;
            dest[nCurOff] |= ((v >> 22) & 63) << nFrameBits;
            adj = 16;
            break;
        case 12:
            dest[nCurOff] |= ((v) & 127) << nFrameBits;
            nCurOff += ((v >> 7) & 127) + 1;
            dest[nCurOff] |= ((v >> 14) & 127) << nFrameBits;
            nCurOff += ((v >> 21) & 127) + 1;
            adj = 0;
            break;
        case 13:
            dest[nCurOff] |= ((v) & 1023) << nFrameBits;
            nCurOff += ((v >> 10) & 511) + 1;
            dest[nCurOff] |= ((v >> 19) & 511) << nFrameBits;
            adj = 16;
            break;
        case 14:
            dest[nCurOff] |= ((v) & 16383) << nFrameBits;
            nCurOff += ((v >> 14) & 16383) + 1;
            adj = 0;
            break;
        case 15:
            dest[nCurOff] |= ((v) & ((1 << 28)-1)) << nFrameBits;
            adj = 16;
            break;
        case 16:
            nCurOff += ((v) & 1) + 1;
            dest[nCurOff] |= ((v >> 1) & 1) << nFrameBits;
            nCurOff += ((v >> 2) & 1) + 1;
            dest[nCurOff] |= ((v >> 3) & 1) << nFrameBits;
            nCurOff += ((v >> 4) & 1) + 1;
            dest[nCurOff] |= ((v >> 5) & 1) << nFrameBits;
            nCurOff += ((v >> 6) & 1) + 1;
            dest[nCurOff] |= ((v >> 7) & 1) << nFrameBits;
            nCurOff += ((v >> 8) & 1) + 1;
            dest[nCurOff] |= ((v >> 9) & 1) << nFrameBits;
            nCurOff += ((v >> 10) & 1) + 1;
            dest[nCurOff] |= ((v >> 11) & 1) << nFrameBits;
            nCurOff += ((v >> 12) & 1) + 1;
            dest[nCurOff] |= ((v >> 13) & 1) << nFrameBits;
            nCurOff += ((v >> 14) & 1) + 1;
            dest[nCurOff] |= ((v >> 15) & 1) << nFrameBits;
            nCurOff += ((v >> 16) & 1) + 1;
            dest[nCurOff] |= ((v >> 17) & 1) << nFrameBits;
            nCurOff += ((v >> 18) & 1) + 1;
            dest[nCurOff] |= ((v >> 19) & 1) << nFrameBits;
            nCurOff += ((v >> 20) & 1) + 1;
            dest[nCurOff] |= ((v >> 21) & 1) << nFrameBits;
            nCurOff += ((v >> 22) & 1) + 1;
            dest[nCurOff] |= ((v >> 23) & 1) << nFrameBits;
            nCurOff += ((v >> 24) & 1) + 1;
            dest[nCurOff] |= ((v >> 25) & 1) << nFrameBits;
            nCurOff += ((v >> 26) & 1) + 1;
            dest[nCurOff] |= ((v >> 27) & 1) << nFrameBits;
            adj = 16;
            break;
        case 17:
            nCurOff += ((v) & 3) + 1;
            dest[nCurOff] |= ((v >> 2) & 3) << nFrameBits;
            nCurOff += ((v >> 4) & 3) + 1;
            dest[nCurOff] |= ((v >> 6) & 3) << nFrameBits;
            nCurOff += ((v >> 8) & 3) + 1;
            dest[nCurOff] |= ((v >> 10) & 3) << nFrameBits;
            nCurOff += ((v >> 12) & 3) + 1;
            dest[nCurOff] |= ((v >> 14) & 1) << nFrameBits;
            nCurOff += ((v >> 15) & 1) + 1;
            dest[nCurOff] |= ((v >> 16) & 1) << nFrameBits;
            nCurOff += ((v >> 17) & 1) + 1;
            dest[nCurOff] |= ((v >> 18) & 1) << nFrameBits;
            nCurOff += ((v >> 19) & 1) + 1;
            dest[nCurOff] |= ((v >> 20) & 1) << nFrameBits;
            nCurOff += ((v >> 21) & 1) + 1;
            dest[nCurOff] |= ((v >> 22) & 1) << nFrameBits;
            nCurOff += ((v >> 23) & 1) + 1;
            dest[nCurOff] |= ((v >> 24) & 1) << nFrameBits;
            nCurOff += ((v >> 25) & 1) + 1;
            dest[nCurOff] |= ((v >> 26) & 1) << nFrameBits;
            nCurOff += ((v >> 27) & 1) + 1;
            adj = 0;
            break;
        case 18:
            nCurOff += ((v) & 1) + 1;
            dest[nCurOff] |= ((v >> 1) & 1) << nFrameBits;
            nCurOff += ((v >> 2) & 1) + 1;
            dest[nCurOff] |= ((v >> 3) & 1) << nFrameBits;
            nCurOff += ((v >> 4) & 1) + 1;
            dest[nCurOff] |= ((v >> 5) & 1) << nFrameBits;
            nCurOff += ((v >> 6) & 1) + 1;
            dest[nCurOff] |= ((v >> 7) & 3) << nFrameBits;
            nCurOff += ((v >> 9) & 3) + 1;
            dest[nCurOff] |= ((v >> 11) & 3) << nFrameBits;
            nCurOff += ((v >> 13) & 3) + 1;
            dest[nCurOff] |= ((v >> 15) & 3) << nFrameBits;
            nCurOff += ((v >> 17) & 3) + 1;
            dest[nCurOff] |= ((v >> 19) & 3) << nFrameBits;
            nCurOff += ((v >> 21) & 1) + 1;
            dest[nCurOff] |= ((v >> 22) & 1) << nFrameBits;
            nCurOff += ((v >> 23) & 1) + 1;
            dest[nCurOff] |= ((v >> 24) & 1) << nFrameBits;
            nCurOff += ((v >> 25) & 1) + 1;
            dest[nCurOff] |= ((v >> 26) & 1) << nFrameBits;
            nCurOff += ((v >> 27) & 1) + 1;
            adj = 0;
            break;
        case 19:
            nCurOff += ((v) & 1) + 1;
            dest[nCurOff] |= ((v >> 1) & 1) << nFrameBits;
            nCurOff += ((v >> 2) & 1) + 1;
            dest[nCurOff] |= ((v >> 3) & 1) << nFrameBits;
            nCurOff += ((v >> 4) & 1) + 1;
            dest[nCurOff] |= ((v >> 5) & 1) << nFrameBits;
            nCurOff += ((v >> 6) & 1) + 1;
            dest[nCurOff] |= ((v >> 7) & 1) << nFrameBits;
            nCurOff += ((v >> 8) & 1) + 1;
            dest[nCurOff] |= ((v >> 9) & 1) << nFrameBits;
            nCurOff += ((v >> 10) & 1) + 1;
            dest[nCurOff] |= ((v >> 11) & 1) << nFrameBits;
            nCurOff += ((v >> 12) & 1) + 1;
            dest[nCurOff] |= ((v >> 13) & 1) << nFrameBits;
            nCurOff += ((v >> 14) & 3) + 1;
            dest[nCurOff] |= ((v >> 16) & 3) << nFrameBits;
            nCurOff += ((v >> 18) & 3) + 1;
            dest[nCurOff] |= ((v >> 20) & 3) << nFrameBits;
            nCurOff += ((v >> 22) & 3) + 1;
            dest[nCurOff] |= ((v >> 24) & 3) << nFrameBits;
            nCurOff += ((v >> 26) & 3) + 1;
            adj = 0;
            break;
        case 20:
            nCurOff += ((v) & 3) + 1;
            dest[nCurOff] |= ((v >> 2) & 3) << nFrameBits;
            nCurOff += ((v >> 4) & 3) + 1;
            dest[nCurOff] |= ((v >> 6) & 3) << nFrameBits;
            nCurOff += ((v >> 8) & 3) + 1;
            dest[nCurOff] |= ((v >> 10) & 3) << nFrameBits;
            nCurOff += ((v >> 12) & 3) + 1;
            dest[nCurOff] |= ((v >> 14) & 3) << nFrameBits;
            nCurOff += ((v >> 16) & 3) + 1;
            dest[nCurOff] |= ((v >> 18) & 3) << nFrameBits;
            nCurOff += ((v >> 20) & 3) + 1;
            dest[nCurOff] |= ((v >> 22) & 3) << nFrameBits;
            nCurOff += ((v >> 24) & 3) + 1;
            dest[nCurOff] |= ((v >> 26) & 3) << nFrameBits;
            adj = 16;
            break;
        case 21:
            nCurOff += ((v) & 15) + 1;
            dest[nCurOff] |= ((v >> 4) & 7) << nFrameBits;
            nCurOff += ((v >> 7) & 7) + 1;
            dest[nCurOff] |= ((v >> 10) & 7) << nFrameBits;
            nCurOff += ((v >> 13) & 7) + 1;
            dest[nCurOff] |= ((v >> 16) & 7) << nFrameBits;
            nCurOff += ((v >> 19) & 7) + 1;
            dest[nCurOff] |= ((v >> 22) & 7) << nFrameBits;
            nCurOff += ((v >> 25) & 7) + 1;
            adj = 0;
            break;
        case 22:
            nCurOff += ((v) & 7) + 1;
            dest[nCurOff] |= ((v >> 3) & 15) << nFrameBits;
            nCurOff += ((v >> 7) & 15) + 1;
            dest[nCurOff] |= ((v >> 11) & 15) << nFrameBits;
            nCurOff += ((v >> 15) & 15) + 1;
            dest[nCurOff] |= ((v >> 19) & 7) << nFrameBits;
            nCurOff += ((v >> 22) & 7) + 1;
            dest[nCurOff] |= ((v >> 25) & 7) << nFrameBits;
            adj = 16;
            break;
        case 23:
            nCurOff += ((v) & 15) + 1;
            dest[nCurOff] |= ((v >> 4) & 15) << nFrameBits;
            nCurOff += ((v >> 8) & 15) + 1;
            dest[nCurOff] |= ((v >> 12) & 15) << nFrameBits;
            nCurOff += ((v >> 16) & 15) + 1;
            dest[nCurOff] |= ((v >> 20) & 15) << nFrameBits;
            nCurOff += ((v >> 24) & 15) + 1;
            adj = 0;
            break;
        case 24:
            nCurOff += ((v) & 31) + 1;
            dest[nCurOff] |= ((v >> 5) & 31) << nFrameBits;
            nCurOff += ((v >> 10) & 31) + 1;
            dest[nCurOff] |= ((v >> 15) & 31) << nFrameBits;
            nCurOff += ((v >> 20) & 15) + 1;
            dest[nCurOff] |= ((v >> 24) & 15) << nFrameBits;
            adj = 16;
            break;
        case 25:
            nCurOff += ((v) & 15) + 1;
            dest[nCurOff] |= ((v >> 4) & 15) << nFrameBits;
            nCurOff += ((v >> 8) & 31) + 1;
            dest[nCurOff] |= ((v >> 13) & 31) << nFrameBits;
            nCurOff += ((v >> 18) & 31) + 1;
            dest[nCurOff] |= ((v >> 23) & 31) << nFrameBits;
            adj = 16;
            break;
        case 26:
            nCurOff += ((v) & 63) + 1;
            dest[nCurOff] |= ((v >> 6) & 63) << nFrameBits;
            nCurOff += ((v >> 12) & 63) + 1;
            dest[nCurOff] |= ((v >> 18) & 31) << nFrameBits;
            nCurOff += ((v >> 23) & 31) + 1;
            adj = 0;
            break;
        case 27:
            nCurOff += ((v) & 31) + 1;
            dest[nCurOff] |= ((v >> 5) & 31) << nFrameBits;
            nCurOff += ((v >> 10) & 63) + 1;
            dest[nCurOff] |= ((v >> 16) & 63) << nFrameBits;
            nCurOff += ((v >> 22) & 63) + 1;
            adj = 0;
            break;
        case 28:
            nCurOff += ((v) & 127) + 1;
            dest[nCurOff] |= ((v >> 7) & 127) << nFrameBits;
            nCurOff += ((v >> 14) & 127) + 1;
            dest[nCurOff] |= ((v >> 21) & 127) << nFrameBits;
            adj = 16;
            break;
        case 29:
            nCurOff += ((v) & 1023) + 1;
            dest[nCurOff] |= ((v >> 10) & 511) << nFrameBits;
            nCurOff += ((v >> 19) & 511) + 1;
            adj = 0;
            break;
        case 30:
            nCurOff += ((v) & 16383) + 1;
            dest[nCurOff] |= ((v >> 14) & 16383) << nFrameBits;
            adj = 16;
            break;
        case 31:
            nCurOff += ((v) & ((1 << 28)-1)) + 1;
            adj = 0;
            break;

        } // end switch
    } // end for
}


FX_NS_END

#endif //__FX_OPTPFORDELTACODING_H

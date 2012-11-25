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
// Created : 2011-06-25 17:45:14

#ifndef __FX_GZIPDOCUMENTREADER_H
#define __FX_GZIPDOCUMENTREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/processor/PlainDocumentReader.h"
#include "firtex/utility/BinaryFile.h"

//struct z_stream;
struct z_stream_s;

FX_NS_DEF(processor);

struct GZipHeader;

class GZipDocumentReader : public PlainDocumentReader
{
public:
    const static size_t DEFAULT_BUFFER_SIZE = 512 * 1024; //512k

public:
    GZipDocumentReader(size_t nBuffSize = 0);
    GZipDocumentReader(const GZipDocumentReader& src);
    ~GZipDocumentReader();

public:
    /**
     * Open specified plain file
     *
     * @param sFilePath file path 
     * @throw throw FileIOException if open failed.
     */
    virtual void open(const std::string& sFilePath);

    /// Return true if no more bytes
    virtual bool isEof();
    
    /**
     * Read max nBytes to pData
     *
     * @param pData data read to
     * @param nBytes number of bytes to read
     *
     * @return number of bytes read
     * @throw throw FileIOException if open failed.
     */
    virtual size_t read(void* pData, size_t nBytes);

    /// Tell the current read position
    virtual int64_t tell();

    /// Return file path
    virtual std::string getFilePath() const;

private:
    bool checkHeader(const GZipHeader& header);

    bool getBytes(char* buff, size_t nBytes);
    bool refillBuffer();

private:
    FX_NS(utility)::BinaryFile m_file;
    int64_t m_nFileLength;

    z_stream_s* m_pZStream;

    char* m_pBuffer;
    size_t m_nBuffSize;
    size_t m_nBytesInBuffer;
    size_t m_nReadUpTo;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////
///
inline bool GZipDocumentReader::getBytes(char* buff, size_t nBytes)
{
    int32_t left = (int32_t)(m_nBytesInBuffer - m_nReadUpTo);
    if (left >= (int32_t)nBytes)
    {
        memcpy(buff, m_pBuffer + m_nReadUpTo, nBytes);
        m_nReadUpTo += nBytes;
    }
    else 
    {
        size_t n = nBytes;
        if (left > 0)
        {
            memcpy(buff, m_pBuffer + m_nReadUpTo, (size_t)left);
            n -= (size_t)left;
        }
        
        if (!refillBuffer())
        {
            return false;
        }
        
        if (m_nBytesInBuffer < n)
        {
            return false;
        }
        memcpy(buff + left, m_pBuffer + m_nReadUpTo, n);
        m_nReadUpTo += n;
    }

    return true;
}

inline bool GZipDocumentReader::refillBuffer()
{
    m_nBytesInBuffer = m_file.read(m_pBuffer, m_nBuffSize);
    m_nReadUpTo = 0;
    return (m_nBytesInBuffer > 0);
}

FX_NS_END

#endif //__FX_GZIPDOCUMENTREADER_H

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
// Created : 2011-06-21 19:17:28

#ifndef __FX_PLAINDOCUMENTREADER_H
#define __FX_PLAINDOCUMENTREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/BinaryFile.h"
#include "firtex/extension/processor/DocumentReader.h"

FX_NS_DEF(processor);

class PlainDocumentReader : public DocumentReader
{
public:
    PlainDocumentReader();
    ~PlainDocumentReader();

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
    FX_NS(utility)::BinaryFile m_file;
    int64_t m_nFileLength;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(PlainDocumentReader);

FX_NS_END

#endif //__FX_PLAINDOCUMENTREADER_H

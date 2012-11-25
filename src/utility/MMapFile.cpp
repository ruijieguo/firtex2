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
// Created	: 2007/4/23
//
#include "firtex/utility/MMapFile.h"
#include "firtex/utility/Exception.h"

#if defined(FX_WINDOWS)
#include "firtex/utility/MMapFile_WIN32.h"
#else
#include "MMapFile_POSIX.cpp"
#endif

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, MMapFile);

MMapFile::MMapFile()
    : m_pImpl(NULL)
{
}

MMapFile::~MMapFile()
{
    close();
}

void MMapFile::open(const std::string& sFileName, std::size_t size, 
                    AccessMode mode, const void* pAddrHint, bool bServer)
{
    FIRTEX_ASSERT2(!m_pImpl);
    m_pImpl = new MMapFileImpl();
    m_pImpl->open(sFileName, size, mode, pAddrHint, bServer);
}

void MMapFile::open(const File& file, AccessMode mode, const void* pAddrHint)
{
    FIRTEX_ASSERT2(!m_pImpl);
    m_pImpl = new MMapFileImpl();
    m_pImpl->open(file, mode, pAddrHint);
}

void MMapFile::open(const FileHandle& fileHandle,
                    AccessMode mode,
                    size_t nMapSize,
                    const void* pAddrHint)
{
    FIRTEX_ASSERT2(!m_pImpl);
    m_pImpl = new MMapFileImpl();
    m_pImpl->open(fileHandle, mode, nMapSize, pAddrHint);
}

MMapFile& MMapFile::operator = (const MMapFile& other)
{
    MMapFile tmp(other);
    swap(tmp);
    return *this;
}

char* MMapFile::begin() const
{
    if (m_pImpl)
        return m_pImpl->begin();
    else
        return 0;
}

char* MMapFile::end() const
{
    if (m_pImpl)
        return m_pImpl->end();
    else
        return 0;
}

int64_t MMapFile::length() const
{
    return m_pImpl->length();
}

std::string MMapFile::getFileName() const
{
    return m_pImpl->getFileName();
}


void MMapFile::close()
{
    if (m_pImpl)
    {
        m_pImpl->release();
        m_pImpl = NULL;
    }
}

FX_NS_END


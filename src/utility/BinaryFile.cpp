/**
 * Copyright (C) 2007 Guo Ruijie. All rights reserved.
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 *
 */

#include <sys/stat.h>
#include <fcntl.h>

#include "firtex/utility/BinaryFile.h"
#include "firtex/utility/Exception.h"

#if defined FX_WINDOWS
#include "firtex/utility/BinaryFile_WIN32.h"
#elif defined FX_POSIX || defined FX_MACOS
#include "firtex/utility/BinaryFile_POSIX.h"
#else
#error "No BinaryFile implement."
#endif

#if _MSC_VER == 1400
#pragma warning(push)
#pragma warning( disable : 4290)
#endif

FX_NS_DEF(utility)

BinaryFile::BinaryFile() : m_pImpl (NULL)
{
}

BinaryFile::~BinaryFile()
{
	if (m_pImpl != NULL)
    {
        delete m_pImpl;
        m_pImpl = NULL;
    }    
}

void BinaryFile::open(const tstring &sFileName,
                      BinaryFile::OpenMode mode) throw(FileIOException)
{
	// File must be closed.
	if (m_pImpl != NULL) 
		throw FileIOException(sFileName + _T(": open failed."));
    m_pImpl = new BinaryFileImpl();
    m_pImpl->open(sFileName, mode);
}

void BinaryFile::seek(int64_t nPos) throw(FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    m_pImpl->seek(nPos);
}

void BinaryFile::seekRelative(int64_t nRelPos) throw(FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    m_pImpl->seekRelative(nRelPos);
}

int64_t BinaryFile::getPos(void) const throw(FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    return m_pImpl->getPos();    
}

size_t BinaryFile::read(void* pData,size_t nBytes) throw(FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    return m_pImpl->read(pData, nBytes);
}

void BinaryFile::readExact(void* pData, size_t nBytes) throw(FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    m_pImpl->readExact(pData, nBytes);    
}

void BinaryFile::write(const void* pData,size_t nBytes) throw(FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    m_pImpl->write(pData, nBytes);
}

void BinaryFile::pread(void* pData,size_t nBytes,int64_t nPos) throw(FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    m_pImpl->pread(pData, nBytes, nPos);
}
	
void BinaryFile::pwrite(void* pData,size_t nBytes,int64_t nPos) throw(FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    m_pImpl->pwrite(pData, nBytes, nPos);
}
	
void BinaryFile::close(void) throw (FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    m_pImpl->close();
}

void BinaryFile::flush(void) throw (FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    m_pImpl->flush();
}

void BinaryFile::truncate(int64_t nNewSize) throw (FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    m_pImpl->truncate(nNewSize);
}

int64_t BinaryFile::getLength(void) throw (FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    return m_pImpl->getLength();
}

bool BinaryFile::isFileOpen(void) const
{
   return (m_pImpl && m_pImpl->isFileOpen());
}

BinaryFile::OpenMode BinaryFile::getFileMode(void) const throw(FileIOException)
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    return m_pImpl->getFileMode();
}

tstring BinaryFile::getFileName(void) const
{
    FIRTEX_ASSERT2(m_pImpl != NULL);
    return m_pImpl->getFileName();
}

FX_NS_END

#if _MSC_VER == 1400
#pragma warning(pop)
#endif

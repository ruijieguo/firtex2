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
#include <sys/types.h>
#include <unistd.h>


#include "firtex/utility/BinaryFile_POSIX.h"
#include "firtex/utility/File.h"

FX_NS_DEF(utility);

// Mode settings for opening files.
#define RDONLY_BITS (O_RDONLY)
#define RDWR_BITS (O_RDWR)
#define CRTR_BITS (O_RDWR|O_CREAT|O_TRUNC)
#define CREATE_BITS (O_RDWR|O_CREAT|O_EXCL)

// PMODE is used when new files are created.
// Otherwise it is ignored. PMODE tells
// the system what file attributes to set
// when the file is created.
#define PMODE (S_IREAD|S_IWRITE)

BinaryFileImpl::BinaryFileImpl()
    : m_handle(-1)
    , m_fileMode((BinaryFile::OpenMode)0)
{
}

BinaryFileImpl::~BinaryFileImpl()
{
    if (m_handle != -1)
        close();
}

void BinaryFileImpl::open(const tstring &filename,
                          BinaryFile::OpenMode mode) throw(FileIOException)
{
    // File must be closed.
    if (m_handle != -1) 
    {
        throw FileIOException(filename + _T(": open failed."));
    }

    m_sFileName = filename;	
	
    // Check file open flags.
    int oflag = 0;
    if (mode == BinaryFile::READ)
        oflag |= RDONLY_BITS;
    else if (mode == BinaryFile::RDWR)
        oflag |= RDWR_BITS;
    else if (mode == BinaryFile::CREATE)
        oflag |= CREATE_BITS;
    else if (mode == BinaryFile::CRTR)
        oflag |= CRTR_BITS;
    else
    {
        throw FileIOException(m_sFileName + _T(": bad open mode."));
    }

    // Start opening file.
    int result = ::open(filename.c_str(), oflag, PMODE);
    if (result == -1)
    {
        m_handle = -1;
        File::handleLastError(m_sFileName);
    }
    else
    {
        m_handle = result;
        m_fileMode = mode;
    }
}

void BinaryFileImpl::seek(int64_t nPos) throw(FileIOException)
{
    FIRTEX_ASSERT( (m_handle != -1),_T("Bad file handle"));
#if defined (FX_MACOS)
    int64_t result = lseek(m_handle,nPos,SEEK_SET);
#else	
    int64_t result = lseek64(m_handle,nPos,SEEK_SET);
#endif
    if (result == -1)
    {
        File::handleLastError(m_sFileName);
    }
}

void BinaryFileImpl::seekRelative(int64_t nRelPos) throw(FileIOException)
{
    FIRTEX_ASSERT( (m_handle != -1),_T("Bad file handle"));
#if defined (FX_MACOS)
    int64_t result = lseek(m_handle,nRelPos,SEEK_SET);
#else	
    int64_t result = lseek64(m_handle,nRelPos,SEEK_CUR);
#endif
    if (result == -1)
    {
        File::handleLastError(m_sFileName);
    }
}

int64_t BinaryFileImpl::getPos(void) const throw(FileIOException)
{
    FIRTEX_ASSERT( (m_handle != -1),_T("Bad file handle"));
	
    int64_t nPos = 0;
#if defined (FX_MACOS)
    int64_t nSeekResult = lseek(m_handle, 0, SEEK_CUR);
#else	
    int64_t nSeekResult = lseek64(m_handle, 0, SEEK_CUR);
#endif
    if (nSeekResult == -1)
    {
        File::handleLastError(m_sFileName);
    }
    else 
    {
        nPos = nSeekResult;
    }
    return nPos;
}

size_t BinaryFileImpl::read(void* pData,size_t nBytes) throw(FileIOException)
{
    FIRTEX_ASSERT( (m_handle != -1),_T("Bad file handle"));
	
    int result = ::read(m_handle, pData, static_cast<int>(nBytes));
    if (result == -1)
    {
        File::handleLastError(m_sFileName);
    }
    return (size_t)result;
}

void BinaryFileImpl::readExact(void* pData, size_t nBytes) throw(FileIOException)
{
    FIRTEX_ASSERT( (m_handle != -1), _T("Bad file handle"));
	
    int result = ::read(m_handle, pData, static_cast<int>(nBytes));
    if (result == -1)
    {
        File::handleLastError(m_sFileName);
    }
    else if (result != static_cast<int>(nBytes))
    {
        throw FileIOException(m_sFileName + _T(": read past EOF."));
    }
}

void BinaryFileImpl::write(const void* pData,size_t nBytes) throw(FileIOException)
{
    FIRTEX_ASSERT( (m_handle != -1),_T("Bad file handle"));

    if (m_handle == -1) 
    {
        FIRTEX_THROW(FileIOException, _T("Write file: [%s] FAILED."), m_sFileName.c_str());
    }
    int result = ::write(m_handle, pData, static_cast<int>(nBytes));
    if (result == -1)
    {
        File::handleLastError(m_sFileName);
    }
    else if (result != static_cast<int>(nBytes)) 
    {
        FIRTEX_THROW(FileIOException, _T("write [%s] FAILED."),
                     m_sFileName.c_str());
    }
}

void BinaryFileImpl::pread(void* pData,size_t nBytes,int64_t nPos) throw(FileIOException)
{
    FIRTEX_ASSERT( (m_handle != 1),_T("Bad file handle"));
#if defined (FX_MACOS)
    int result = ::pread(m_handle,pData,static_cast<int>(nBytes),nPos);
#else	
    int result = pread64(m_handle,pData,static_cast<int>(nBytes),nPos);
#endif
    if (result == -1) 
    {
        File::handleLastError(m_sFileName);
    }
    else if (result != static_cast<int>(nBytes)) 
    {
        throw FileIOException(m_sFileName + _T(": pread failed."));
    }
}
	
void BinaryFileImpl::pwrite(void* pData,size_t nBytes,int64_t nPos) throw(FileIOException)
{
    FIRTEX_ASSERT( (m_handle != -1),_T("Bad file handle"));
#if defined (FX_MACOS)
    int result = ::pwrite(m_handle,pData,static_cast<int>(nBytes),nPos);
#else	
    int result = pwrite64(m_handle,pData,static_cast<int>(nBytes),nPos);
#endif
    if (result == -1)
    {
        File::handleLastError(m_sFileName);
    }
    else if (result != static_cast<int>(nBytes))
    {
        throw FileIOException(m_sFileName + _T(": pwrite failed."));
    }
}
	
void BinaryFileImpl::close(void) throw (FileIOException)
{
    FIRTEX_ASSERT( (m_handle != -1),_T("Bad file handle"));
	
    int result = ::close(m_handle);
    if (result == -1)
    {
        File::handleLastError(m_sFileName);
    }
    else
    {
        m_handle = -1;
    }
}

void BinaryFileImpl::flush(void) throw (FileIOException)
{
    FIRTEX_ASSERT( (m_handle != -1),_T("Bad file handle"));	
    int result = ::close(dup(m_handle));
    if (result == -1)
    {
        File::handleLastError(m_sFileName);
    }
}

void BinaryFileImpl::truncate(int64_t nNewSize) throw (FileIOException)
{
#ifdef HAVE_FTRUNCATE64	
    FIRTEX_ASSERT( (m_handle != -1),_T("Bad file handle"));	
    int result = ftruncate64(m_handle,nNewsize);
    if (result == -1)
    {
        File::handleLastError(m_sFileName);
    }
#else
    throw InvalidOpException(_T("Undefined function ftruncate64")); //#error Undefined function ftruncate64
#endif
}

int64_t BinaryFileImpl::getLength(void) throw (FileIOException)
{	
    FIRTEX_ASSERT( (m_handle != -1),_T("Bad file handle"));
    int64_t nSize = 0;

    struct stat64 statbuf;
    if ( fstat64(m_handle,&statbuf) < 0)
    {
        File::handleLastError(m_sFileName);
    }
    else
    {
        nSize = statbuf.st_size;
    }
    return nSize;
}

FX_NS_END

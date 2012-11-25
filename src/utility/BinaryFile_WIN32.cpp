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

#include <windows.h>
#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "firtex/utility/BinaryFile_WIN32.h"
#include "firtex/utility/File.h"

#if _MSC_VER == 1400
#pragma warning(push)
#pragma warning( disable : 4290)
#endif

FX_NS_DEF(utility)

SETUP_STREAM_LOGGER(utility, BinaryFileImpl);

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
    : m_fileMode((BinaryFile::OpenMode)0)
    , m_handle(NULL)
{
}

BinaryFileImpl::~BinaryFileImpl()
{
    if (m_handle)
    {
        close();
    }
}

void BinaryFileImpl::open(const tstring &filename,
                          BinaryFile::OpenMode mode) throw(FileIOException)
{
    // File must be closed.
    if (m_handle) 
    {
        throw FileIOException(filename + _T(": open FAILED."));
    }

    m_sFileName = filename;

    // Check file open flags.
    DWORD access = 0;
    DWORD createFlags = OPEN_EXISTING;
    DWORD sharedMode = FILE_SHARE_READ;
    if (mode == BinaryFile::READ)
    {
        access = GENERIC_READ;
    }
    else if (mode == BinaryFile::RDWR)
    {
        access = GENERIC_READ | GENERIC_WRITE;
    }
    else if (mode == BinaryFile::CREATE)
    {
        access = GENERIC_WRITE;
        createFlags = OPEN_ALWAYS;
    }
    else if (mode == BinaryFile::CRTR)
    {
        access = GENERIC_WRITE;
        createFlags = CREATE_ALWAYS;
        sharedMode |= FILE_SHARE_WRITE;
    }
    else
    {
		FIRTEX_THROW(FileIOException, _T("Bad open mode: [%s]"), filename.c_str());
    }
	
    // Start opening file.
    HANDLE hOpenHandle = ::CreateFile(filename.c_str(), access, sharedMode, NULL, 
            createFlags, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
    if (hOpenHandle == INVALID_HANDLE_VALUE)
    {
        m_handle = NULL;
        File::handleLastError(filename);
    }
    else
    {
        m_handle = hOpenHandle;
        m_fileMode = mode;
    }
}

void BinaryFileImpl::seek(int64_t nPos) throw(FileIOException)
{
    FIRTEX_ASSERT( m_handle,_T("Bad file handle"));

    LARGE_INTEGER li;
    li.QuadPart = nPos;
    li.LowPart = ::SetFilePointer(m_handle, li.LowPart, &li.HighPart, FILE_BEGIN);
    if (li.LowPart == INVALID_SET_FILE_POINTER && ::GetLastError() != NO_ERROR)
    {
        File::handleLastError(m_sFileName);
    }
}

void BinaryFileImpl::seekRelative(int64_t nRelPos) throw(FileIOException)
{
    FIRTEX_ASSERT( m_handle,_T("Bad file handle"));

    LARGE_INTEGER li;
    li.QuadPart = nRelPos;
    li.LowPart = ::SetFilePointer(m_handle, li.LowPart, &li.HighPart, FILE_CURRENT);
    if (li.LowPart == INVALID_SET_FILE_POINTER && ::GetLastError() != NO_ERROR)
    {
        File::handleLastError(m_sFileName);
    }
}

int64_t BinaryFileImpl::getPos(void) const throw(FileIOException)
{
    FIRTEX_ASSERT( m_handle,_T("Bad file handle"));
    LARGE_INTEGER li;
    li.QuadPart = 0;
    li.LowPart  = ::SetFilePointer(m_handle, li.LowPart, &li.HighPart, FILE_CURRENT);
    if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
    {
        File::handleLastError(m_sFileName);
    }
    int64_t nPos = li.QuadPart;
    return nPos;
}

size_t BinaryFileImpl::read(void* pData, size_t nBytes) throw(FileIOException)
{
    FIRTEX_ASSERT( m_handle,_T("Bad file handle"));
    DWORD nBytesToRead = (DWORD)nBytes, nBytesRead;
    BOOL result = ::ReadFile(m_handle, pData, nBytesToRead, &nBytesRead,NULL);
    if (!result) 
    {
        File::handleLastError(m_sFileName);
    }
    return nBytesRead;
}

void BinaryFileImpl::readExact(void* pData, size_t nBytes) throw(FileIOException)
{
    FIRTEX_ASSERT( m_handle, _T("Bad file handle"));
    DWORD nBytesToRead = (DWORD)nBytes, nBytesRead;
    BOOL result = ::ReadFile(m_handle, pData, nBytesToRead, &nBytesRead, NULL);
    if (!result) 
    {
        File::handleLastError(m_sFileName);
    }
    else if (nBytesToRead != nBytesRead)
    {
        throw FileIOException(m_sFileName + _T(": read past EOF."));
    }
}

void BinaryFileImpl::write(const void* pData,size_t nBytes) throw(FileIOException)
{
    FIRTEX_ASSERT( m_handle,_T("Bad file handle"));

    DWORD nBytesToWrite = (DWORD)nBytes,nBytesWritten;
    BOOL result = ::WriteFile(m_handle, pData, nBytesToWrite, &nBytesWritten, NULL);
    if (!result)
    {
        File::handleLastError(m_sFileName);
    }
    else if (nBytesToWrite != nBytesWritten)
    {
        FIRTEX_THROW(FileIOException, _T("Write file: [%s] FAILED."), m_sFileName.c_str());
    }
}

void BinaryFileImpl::pread(void* pData,size_t nBytes,int64_t nPos) throw(FileIOException)
{
    FIRTEX_ASSERT( m_handle,_T("Bad file handle"));

    DWORD nBytesToRead = (DWORD)nBytes,nBytesRead;
    OVERLAPPED ov;
    ov.Offset = (DWORD)(nPos & 0xffffffff);
    ov.OffsetHigh = (DWORD)(nPos >> 32);
    ov.hEvent = 0;
    BOOL result = ::ReadFile(m_handle,pData,nBytesToRead,&nBytesRead,&ov);
    if (!result) 
    {
        File::handleLastError(m_sFileName);
    }
    else if (nBytesToRead != nBytesRead)
    {
        throw FileIOException(m_sFileName + _T(": read failed."));
    }
}
	
void BinaryFileImpl::pwrite(void* pData,size_t nBytes,int64_t nPos) throw(FileIOException)
{
    FIRTEX_ASSERT( m_handle,_T("Bad file handle"));

    DWORD nBytesToWrite = (DWORD)nBytes,nBytesWritten;
    OVERLAPPED ov;
    ov.Offset = (DWORD)(nPos & 0xffffffff);
    ov.OffsetHigh = (DWORD)(nPos >> 32);
    ov.hEvent = 0;
    BOOL result = ::WriteFile(m_handle,pData,nBytesToWrite,&nBytesWritten,&ov);
    if (!result)
    {
        File::handleLastError(m_sFileName);
    }
    else if (nBytesToWrite != nBytesWritten) 
    {
        throw FileIOException(m_sFileName + _T(": write failed."));
    }
}
	
void BinaryFileImpl::close(void) throw (FileIOException)
{
    FIRTEX_ASSERT( m_handle,_T("Bad file handle"));

    BOOL result = ::CloseHandle(m_handle);
    if (!result)
    {
        File::handleLastError(m_sFileName);
    }
    m_handle = NULL;
}

void BinaryFileImpl::flush(void) throw (FileIOException)
{
    FIRTEX_ASSERT( m_handle,_T("Bad file handle"));
    BOOL result = ::FlushFileBuffers(m_handle);
    if (!result)
    {
        File::handleLastError(m_sFileName);
    }
}

void BinaryFileImpl::truncate(int64_t nNewSize) throw (FileIOException)
{
    seek(nNewSize);
    BOOL result = ::SetEndOfFile(m_handle);
    if (!result) 
    {
        File::handleLastError(m_sFileName);
    }
}

int64_t BinaryFileImpl::getLength(void) throw (FileIOException)
{	
    FIRTEX_ASSERT( m_handle,_T("Bad file handle"));
    int64_t nSize = 0;

    BY_HANDLE_FILE_INFORMATION fileinfo;
    BOOL result = ::GetFileInformationByHandle(m_handle,&fileinfo);
    if (!result) 
    {
        File::handleLastError(m_sFileName);
    }
    else
    {
        int32_t* pHigh = (int32_t*) &fileinfo.nFileSizeHigh;
        nSize = *pHigh;
        nSize <<= 32;
        nSize |= fileinfo.nFileSizeLow;
    }
    return nSize;
}

FX_NS_END

#if _MSC_VER == 1400
#pragma warning(pop)
#endif

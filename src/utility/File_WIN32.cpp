#include "firtex/utility/File_WIN32.h"
#include "firtex/utility/Exception.h"
#include <windows.h>

FX_NS_DEF(utility)

class FileHandle
{
public:
    FileHandle(const tstring& path, DWORD access, DWORD share, DWORD disp)
    {
        m_handle = CreateFileA(path.c_str(), access, share, 0, disp, 0, 0);
        if (!m_handle) FileImpl::handleLastError(path);
    }
	
    ~FileHandle()
    {
        if (m_handle) CloseHandle(m_handle);
    }
	
    HANDLE get() const
    {
        return m_handle;
    }
	
private:
    HANDLE m_handle;
};

void FileImpl::transPath(tstring& sPath)
{
    tstring::size_type n = sPath.size();
    if (n > 1 && (sPath[n - 1] == '\\'
                  || sPath[n - 1] == '/')&& !((n == 3 && sPath[1]==':')))
    {
        sPath.resize(n - 1);
    }
}

bool FileImpl::exists(const tstring& sFile)
{
    FIRTEX_ASSERT2(!sFile.empty());

    DWORD attr = GetFileAttributes(sFile.c_str());
    if (attr == 0xFFFFFFFF)
    {
        switch (GetLastError())
        {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
        case ERROR_NOT_READY:
        case ERROR_INVALID_DRIVE:
            return false;
        default:
            handleLastError(sFile);
        }
    }
    return true;
}

bool FileImpl::isFile(const tstring& sPath)
{
    FIRTEX_ASSERT2(!sPath.empty());

    DWORD attr = GetFileAttributes(sPath.c_str());
    if (attr == 0xFFFFFFFF)
        handleLastError(sPath);
    return (attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
}


bool FileImpl::isDirectory(const tstring& sPath)
{
    FIRTEX_ASSERT2(!sPath.empty());

    DWORD attr = GetFileAttributes(sPath.c_str());
    if (attr == 0xFFFFFFFF)
    {
        handleLastError(sPath);
    }
    return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool FileImpl::isLink(const tstring& sPath)
{
    return false;
}

Timestamp FileImpl::created(const tstring& sPath)
{
    FIRTEX_ASSERT2(!sPath.empty());

    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (GetFileAttributesEx(sPath.c_str(), GetFileExInfoStandard, &fad) == 0)
        handleLastError(sPath);
    return Timestamp::fromFileTimeNP(fad.ftCreationTime.dwLowDateTime,
            fad.ftCreationTime.dwHighDateTime);
}

Timestamp FileImpl::lastModified(const tstring& sPath)
{
    FIRTEX_ASSERT2(!sPath.empty());

    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (GetFileAttributesEx(sPath.c_str(), GetFileExInfoStandard, &fad) == 0) 
        handleLastError(sPath);
    return Timestamp::fromFileTimeNP(fad.ftLastWriteTime.dwLowDateTime,
            fad.ftLastWriteTime.dwHighDateTime);
}

void FileImpl::setLastModified(const tstring& sPath, const Timestamp& ts)
{
    FIRTEX_ASSERT2(!sPath.empty());

    uint32_t low;
    uint32_t high;
    ts.toFileTimeNP(low, high);
    FILETIME ft;
    ft.dwLowDateTime  = low;
    ft.dwHighDateTime = high;
    FileHandle fh(sPath, FILE_ALL_ACCESS, FILE_SHARE_WRITE, OPEN_EXISTING);
    if (SetFileTime(fh.get(), 0, &ft, &ft) == 0)
        handleLastError(sPath);
}

FileImpl::FileSizeImpl FileImpl::getSize(const tstring& sPath)
{
    FIRTEX_ASSERT2(!sPath.empty());

    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (GetFileAttributesEx(sPath.c_str(), GetFileExInfoStandard, &fad) == 0) 
        handleLastError(sPath);
    LARGE_INTEGER li;
    li.LowPart  = fad.nFileSizeLow;
    li.HighPart = fad.nFileSizeHigh;
    return li.QuadPart;
}

void FileImpl::setSize(const tstring& sPath, FileSizeImpl size)
{
    FIRTEX_ASSERT2(!sPath.empty());

    FileHandle fh(sPath, GENERIC_WRITE, FILE_SHARE_WRITE, OPEN_EXISTING);
    LARGE_INTEGER li;
    li.QuadPart = size;
    if (SetFilePointer(fh.get(), li.LowPart, &li.HighPart, FILE_BEGIN) == -1)
        handleLastError(sPath);
    if (SetEndOfFile(fh.get()) == 0) 
        handleLastError(sPath);
}

void FileImpl::setWriteable(const tstring& sPath, bool flag)
{
    FIRTEX_ASSERT2(!sPath.empty());

    DWORD attr = GetFileAttributes(sPath.c_str());
    if (attr == -1)
        handleLastError(sPath);
    if (flag)
        attr &= ~FILE_ATTRIBUTE_READONLY;
    else
        attr |= FILE_ATTRIBUTE_READONLY;
    if (SetFileAttributes(sPath.c_str(), attr) == 0)
        handleLastError(sPath);
}

void FileImpl::copyTo(const tstring& sPath, const tstring& sDestPath)
{
    FIRTEX_ASSERT2(!sPath.empty());

    if (CopyFile(sPath.c_str(), sDestPath.c_str(), FALSE) != 0) 
    {
        FileImpl::setWriteable(sDestPath, true);
    }
    else handleLastError(sPath);    
}

void FileImpl::renameTo(const tstring& sPath, const tstring& sDestPath)
{
    FIRTEX_ASSERT2(!sPath.empty());

    if (MoveFile(sPath.c_str(), sDestPath.c_str()) == 0) 
        handleLastError(sPath);
}

void FileImpl::remove(const tstring& sPath)
{
    FIRTEX_ASSERT2(!sPath.empty());

    if (isDirectory(sPath))
    {
        if (RemoveDirectory(sPath.c_str()) == 0) 
            handleLastError(sPath);
    }
    else
    {
        if (DeleteFile(sPath.c_str()) == 0)
            handleLastError(sPath);
    }
}

bool FileImpl::createFile(const tstring& sPath)
{
    FIRTEX_ASSERT2(!sPath.empty());

    HANDLE hFile = CreateFile(sPath.c_str(), GENERIC_WRITE, 0, 0,
                              CREATE_NEW, 0, 0);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
        return true;
    }
    else if (GetLastError() == ERROR_FILE_EXISTS)
        return false;
    else
        handleLastError(sPath);
    return false;
}

bool FileImpl::createDirectory(const tstring& sPath)
{
    FIRTEX_ASSERT2(!sPath.empty());
	
    if (exists(sPath) && isDirectory(sPath))
        return false;
    if (CreateDirectory(sPath.c_str(), 0) == 0)
        handleLastError(sPath);
    return true;
}

void FileImpl::handleLastError(const tstring& sPath)
{
    DWORD ret = GetLastError();
    switch (ret)
    {
    case ERROR_FILE_NOT_FOUND:
        FIRTEX_THROW(FileIOException, _T("File not found.: [%s]"), sPath.c_str());
    case ERROR_PATH_NOT_FOUND:
    case ERROR_BAD_NETPATH:
    case ERROR_CANT_RESOLVE_FILENAME:
    case ERROR_INVALID_DRIVE:
        FIRTEX_THROW(FileIOException, _T("Invalid drive.: [%s]"), sPath.c_str());
    case ERROR_ACCESS_DENIED:
        FIRTEX_THROW(FileIOException, _T("Access denied.: [%s]"), sPath.c_str());
    case ERROR_ALREADY_EXISTS:
		FIRTEX_THROW(FileIOException, _T("Cannot create a file when that file already exists: [%s]"), sPath.c_str());
    case ERROR_FILE_EXISTS:
        FIRTEX_THROW(FileIOException, _T("File is not exist.: [%s]"), sPath.c_str());
    case ERROR_INVALID_NAME:
    case ERROR_DIRECTORY:
    case ERROR_FILENAME_EXCED_RANGE:
    case ERROR_BAD_PATHNAME:
        FIRTEX_THROW(FileIOException, _T("Bad path name.: [%s]"), sPath.c_str());
    case ERROR_FILE_READ_ONLY:
        FIRTEX_THROW(FileIOException, _T("File is read only: [%s]"), sPath.c_str());
    case ERROR_CANNOT_MAKE:
        FIRTEX_THROW(FileIOException, _T("Create file failed.: [%s]"), sPath.c_str());
    case ERROR_DIR_NOT_EMPTY:
        FIRTEX_THROW(FileIOException, _T("Directory not empty: [%s]"), sPath.c_str());
    case ERROR_WRITE_FAULT:
        FIRTEX_THROW(FileIOException, sPath.c_str());
    case ERROR_READ_FAULT:
        FIRTEX_THROW(FileIOException, sPath.c_str());
    case ERROR_SHARING_VIOLATION:
        FIRTEX_THROW(FileIOException, _T("Sharing violation: [%s]"), sPath.c_str());
    case ERROR_LOCK_VIOLATION:
        FIRTEX_THROW(FileIOException, _T("Lock violation: [%s]"), sPath.c_str());
    case ERROR_HANDLE_EOF:
        FIRTEX_THROW(FileIOException, _T("Eof reached: [%s]"), sPath.c_str() );
    case ERROR_HANDLE_DISK_FULL:
    case ERROR_DISK_FULL:
        FIRTEX_THROW(FileIOException, _T("Disk is full: [%s]"), sPath.c_str());
    case ERROR_NEGATIVE_SEEK:
        FIRTEX_THROW(FileIOException, _T("Negative seek: [%s]"), sPath.c_str());
    default:
        FIRTEX_THROW(FileIOException, "Unknow File Io error.");
	}
}

FX_NS_END

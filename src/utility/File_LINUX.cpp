#include "firtex/utility/File_LINUX.h"
#include "firtex/utility/Exception.h"
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <utime.h>
#include <cstring>

FX_NS_DEF(utility);

void FileImpl::transPath(tstring& sPath)
{
    tstring::size_type n = sPath.size();
    if (n > 1 && sPath[n - 1] == '/')
        sPath.resize(n - 1);
}

bool FileImpl::exists(const tstring& sFile)
{
    FIRTEX_ASSERT2 (!sFile.empty());

    struct stat st;
    return stat(sFile.c_str(), &st) == 0;
}

bool FileImpl::isFile(const tstring& sPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    struct stat st;
    if (stat(sPath.c_str(), &st) == 0)
        return S_ISREG(st.st_mode);
    else
        handleLastError(sPath);
    return false;
}

bool FileImpl::isDirectory(const tstring& sPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    struct stat st;
    if (stat(sPath.c_str(), &st) == 0)
        return S_ISDIR(st.st_mode);
    else
        handleLastError(sPath);
    return false;
}

bool FileImpl::isLink(const tstring& sPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    struct stat st;
    if (lstat(sPath.c_str(), &st) == 0)
        return S_ISLNK(st.st_mode);
    else
        handleLastError(sPath);
    return false;
}

Timestamp FileImpl::created(const tstring& sPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());

#if defined(__APPLE__) && defined(st_birthtime) // st_birthtime is available only on 10.5
    struct stat64 st;
    if (stat64(sPath.c_str(), &st) == 0)
        return Timestamp::fromTime(st.st_birthtime);
#elif defined(__FreeBSD__)
    struct stat st;
    if (stat(sPath.c_str(), &st) == 0)
        return Timestamp::fromTime(st.st_birthtime);
#else
    struct stat st;
    if (stat(sPath.c_str(), &st) == 0)
        return Timestamp::fromTime(st.st_ctime);
#endif 
    else
        handleLastError(sPath);
    return 0;
}

Timestamp FileImpl::lastModified(const tstring& sPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    struct stat st;
    if (stat(sPath.c_str(), &st) == 0)
        return Timestamp::fromTime(st.st_mtime);
    else
        handleLastError(sPath);
    return 0;
}

void FileImpl::setLastModified(const tstring& sPath, const Timestamp& ts)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    struct utimbuf tb;
    tb.actime  = ts.time();
    tb.modtime = ts.time();
    if (utime(sPath.c_str(), &tb) != 0)
        handleLastError(sPath);
}

FileImpl::FileSizeImpl FileImpl::getSize(const tstring& sPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    struct stat st;
    if (stat(sPath.c_str(), &st) == 0)
        return st.st_size;
    else
        handleLastError(sPath);
    return 0;
}

void FileImpl::setSize(const tstring& sPath, FileSizeImpl size)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    if (truncate(sPath.c_str(), size) != 0)
        handleLastError(sPath);
}

void FileImpl::copyTo(const tstring& sPath, const tstring& sDestPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    int sd = open(sPath.c_str(), O_RDONLY);
    if (sd == -1) handleLastError(sPath);

    struct stat st;
    if (fstat(sd, &st) != 0) 
    {
        close(sd);
        handleLastError(sPath);
    }
    const long blockSize = st.st_blksize;

    int dd = open(sDestPath.c_str(), O_CREAT | O_TRUNC | O_WRONLY,
                  st.st_mode & S_IRWXU);
    if (dd == -1)
    {
        close(sd);
        handleLastError(sDestPath);
    }
    char* buffer = new char[blockSize];
    try
    {
        int n;
        while ((n = read(sd, buffer, blockSize)) > 0)
        {
            if (write(dd, buffer, n) != n) 
                handleLastError(sDestPath);
        }
        if (n < 0)
            handleLastError(sPath);
    }
    catch (...)
    {
        delete[] buffer;
        close(sd);
        close(dd);
        throw;
    }
    delete[] buffer;
    close(sd);
    if (fsync(dd) != 0) 
    {
        close(dd);
        handleLastError(sDestPath);
    }
    if (close(dd) != 0)
        handleLastError(sDestPath);
}

void FileImpl::renameTo(const tstring& sPath, const tstring& sDestPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    if (rename(sPath.c_str(), sDestPath.c_str()) != 0)
        handleLastError(sPath);
}

void FileImpl::remove(const tstring& sPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    int rc;
    if (!isLink(sPath) && isDirectory(sPath))
        rc = rmdir(sPath.c_str());
    else
        rc = unlink(sPath.c_str());
    if (rc) handleLastError(sPath);
}

bool FileImpl::createFile(const tstring& sPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());
	
    int n = open(sPath.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (n != -1)
    {
        close(n);
        return true;
    }
    if (n == -1 && errno == EEXIST)
        return false;
    else
        handleLastError(sPath);
    return false;
}

bool FileImpl::createDirectory(const tstring& sPath)
{
    FIRTEX_ASSERT2 (!sPath.empty());

    if (exists(sPath) && isDirectory(sPath))
        return false;
    if (mkdir(sPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) 
        handleLastError(sPath);
    return true;
}

void FileImpl::handleLastError(const tstring& sPath)
{
    switch (errno)
    {
    case EIO:
        FIRTEX_THROW(FileIOException, "Input/output error: [%s]", sPath.c_str());
    case EPERM:
        FIRTEX_THROW(FileIOException, _T("Insufficient permissions: [%s]"), sPath.c_str());
    case EACCES:
        FIRTEX_THROW(FileIOException, _T("Requested access to the file is not allowed: [%s]"), sPath.c_str());
    case ENOENT:
        FIRTEX_THROW(FileIOException, _T("Does not exist: [%s]"), sPath.c_str());
    case ENOTDIR:
        FIRTEX_THROW(FileIOException, _T("Not a directory: [%s]"), sPath.c_str());
    case EISDIR:
        FIRTEX_THROW(FileIOException, _T("Not a file: [%s]"), sPath.c_str());
    case EROFS:
        FIRTEX_THROW(FileIOException, _T("Read-only file system: [%s]"), sPath.c_str());
    case EEXIST:
        FIRTEX_THROW(FileIOException, _T("File exists: [%s]"),sPath.c_str());
    case ENOSPC:
        FIRTEX_THROW(FileIOException, ("No space left on device: [%s]"), sPath.c_str());
    case EDQUOT:
        FIRTEX_THROW(FileIOException, _T("Disk quota exceeded [%s]"), sPath.c_str());
    case ENOTEMPTY:
        FIRTEX_THROW(FileIOException, _T("Directory not empty [%s]"), sPath.c_str());
    case ENAMETOOLONG:
        FIRTEX_THROW(FileIOException, _T("%s"), sPath.c_str());
    case ENFILE:
    case EMFILE:
        FIRTEX_THROW(FileIOException, _T("Too many open files: [%s]"), sPath.c_str());
    case EINVAL:
        FIRTEX_THROW(FileIOException, _T("Invalid argument : [%s]"), sPath.c_str());
    default:
        FIRTEX_THROW(FileIOException, _T("file : [%s], error : [%s]"), sPath.c_str(),
                     std::strerror(errno));
    }
}

FX_NS_END

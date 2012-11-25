#include "firtex/utility/MMapFile_POSIX.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/File.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, MMapFileImpl);

MMapFileImpl::MMapFileImpl()
    : m_nSize(0)
    , m_fd(-1)
    , m_pAddress(0)
    , m_access(MMapFile::AM_UNKNOW)
    , m_bFileMapped(false)
    , m_bServer(false)
    , m_bHandleOwner(false)
{
}

void MMapFileImpl::open(const std::string& name, std::size_t size,
                        MMapFile::AccessMode mode,
                        const void* addrHint, bool server)
{
    m_nSize = size;
    m_access = mode;
    m_bServer = server;
    m_sFileName.append("/");
    m_sFileName.append(name);

    int flags = O_CREAT;
    if (m_access == MMapFile::AM_WRITE)
        flags |= O_RDWR;
    else
        flags = O_RDONLY;

    // open the shared memory segment
    m_fd = ::shm_open(m_sFileName.c_str(), flags, S_IRUSR | S_IWUSR);
    if (m_fd == -1)
    {
        File::handleLastError(m_sFileName);
    }
    
    m_bHandleOwner = true;

    // now set the correct size for the segment
    if ((m_access == MMapFile::AM_WRITE) && (-1 == ::ftruncate(m_fd, size)))
    {
        ::close(m_fd);
        m_fd = -1;
        ::shm_unlink(m_sFileName.c_str());
        File::handleLastError(m_sFileName);
    }
    map(addrHint);
}

void MMapFileImpl::open(const File& file, MMapFile::AccessMode mode,
                        const void* addrHint)
{
    m_access = mode;
    m_sFileName = file.getPath();
    m_bFileMapped = true;
    m_bServer = false;
    m_bHandleOwner = true;

    if (!file.exists() || !file.isFile())
    {
        FIRTEX_THROW(FileIOException, "Open file : [%s] FAILED",
                     file.getPath().c_str());
    }

    m_nSize = file.getSize();
    int flag = O_RDONLY;
    if (mode == MMapFile::AM_WRITE)
        flag = O_RDWR;
    m_fd = ::open(m_sFileName.c_str(), flag);
    if (-1 == m_fd)
    {
        FIRTEX_THROW(FileIOException,
                     _T(":Cannot open memory mapped file: [%s]"),
                             m_sFileName.c_str());
    }

    map(addrHint);
}

void MMapFileImpl::open(const FileHandle& fileHandle, MMapFile::AccessMode mode,
                        size_t nMapSize, const void* addrHint)
{
    if (!FX_OS_IS_VALID_FILE_DESCRIPTOR(fileHandle))
    {
        FIRTEX_THROW(FileIOException, "Invalid file handle");
    }

    m_fd = fileHandle;
    m_access = mode;
    m_nSize = nMapSize;
    m_bHandleOwner = false;
    m_bFileMapped = true;
    
    map(addrHint);
}

MMapFileImpl::~MMapFileImpl()
{
    unmap();
    close();
}

void MMapFileImpl::map(const void* addrHint)
{
    int access = PROT_READ;
    if (m_access == MMapFile::AM_WRITE)
        access |= PROT_WRITE;
    if (m_nSize == 0)
    {
        m_pAddress = NULL;
        return;
    }

    void* addr = ::mmap(const_cast<void*>(addrHint), m_nSize, access, MAP_SHARED, m_fd, 0);
    if (addr == MAP_FAILED)
    {
        File::handleLastError(m_sFileName);
    }

    m_pAddress = static_cast<char*>(addr);
}

void MMapFileImpl::unmap()
{
    if (m_pAddress)
    {
        ::munmap(m_pAddress, m_nSize);
    }
}

void MMapFileImpl::close()
{
    if (m_fd != -1 && m_bHandleOwner)
    {
        ::close(m_fd);
        m_fd = -1;
    }

    if (!m_bFileMapped && !m_bServer)
    {
        ::shm_unlink(m_sFileName.c_str());
    }
}

FX_NS_END

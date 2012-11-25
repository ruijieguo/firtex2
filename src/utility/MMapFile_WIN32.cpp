#include "firtex/utility/MMapFile_WIN32.h"
#include "firtex/utility/Exception.h"
#include <windows.h>

FX_NS_DEF(utility)

SETUP_STREAM_LOGGER(utility, MMapFileImpl);

MMapFileImpl::MMapFileImpl()
    : m_hMapHandle(INVALID_HANDLE_VALUE)
    , m_hFileHandle(INVALID_HANDLE_VALUE)
    , m_nFileSize(0)
    , m_mode(PAGE_READONLY)
    , m_pAddress(0)
    , m_bHandleOwner(false)
{
}

MMapFileImpl::~MMapFileImpl()
{
    unmap();
    close();
}

void MMapFileImpl::open(const std::string& sFileName, std::size_t size, 
                        MMapFile::AccessMode mode, const void*, bool)
{
    m_sFileName = sFileName;
    m_nFileSize = static_cast<int64_t>(size);
    m_mode = PAGE_READONLY;
    m_bHandleOwner = true;

    if (mode == MMapFile::AM_WRITE)
        m_mode = PAGE_READWRITE;
    m_hMapHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, m_mode,
            (DWORD)(m_nFileSize >> 32), (DWORD)m_nFileSize, m_sFileName.c_str());

    if (!m_hMapHandle)
    {
        FIRTEX_THROW(FileIOException, "Cannot create shared memory object: [%s]",
                     m_sFileName.c_str());
    }
    map();
}

void MMapFileImpl::open(const File& file, MMapFile::AccessMode mode, const void*)
{
    m_sFileName = file.getPath();
    m_nFileSize = 0;
    m_mode = PAGE_READONLY;
    m_bHandleOwner = true;

    if (!file.exists() || !file.isFile())
    {
        FIRTEX_THROW(FileIOException, "File not found: [%s]",  m_sFileName.c_str());
    }

    m_nFileSize = static_cast<int64_t>(file.getSize());

    DWORD shareMode = FILE_SHARE_READ;
    DWORD fileMode  = GENERIC_READ;

    if (mode == MMapFile::AM_WRITE)
    {
        m_mode = PAGE_READWRITE;
        fileMode |= GENERIC_WRITE;
    }

    m_hFileHandle = CreateFile(m_sFileName.c_str(), fileMode, shareMode, NULL, OPEN_EXISTING, 
                               FILE_ATTRIBUTE_NORMAL, NULL);

    if (m_hFileHandle == INVALID_HANDLE_VALUE)
    {
		FIRTEX_THROW(FileIOException, "Cannot open memory mapped file: [%s]", m_sFileName.c_str());
    }

    m_hMapHandle = CreateFileMapping(m_hFileHandle, NULL, m_mode, 0, 0, NULL);
    if (!m_hMapHandle)
    {
        CloseHandle(m_hFileHandle);
        m_hFileHandle = INVALID_HANDLE_VALUE;
        throw FileIOException(m_sFileName + ":Cannot map file into shared memory");
    }
    map();
}

void MMapFileImpl::open(const FileHandle& fileHandle, MMapFile::AccessMode mode,
                        size_t nMapSize, const void* addrHint)
{
    if (!FX_OS_IS_VALID_FILE_DESCRIPTOR(fileHandle))
    {
        FIRTEX_THROW(FileIOException, "Invalid file handle");
    }

    m_mode = PAGE_READONLY;
    DWORD shareMode = FILE_SHARE_READ;
    DWORD fileMode  = GENERIC_READ;

    if (mode == MMapFile::AM_WRITE)
    {
        m_mode = PAGE_READWRITE;
        fileMode |= GENERIC_WRITE;
    }

    m_hFileHandle = fileHandle.fileHandle;
    m_nFileSize = (int64_t)nMapSize;
    m_bHandleOwner = false;
    
    if (fileHandle.useFileMapping != FX_OS_FLAG_USE_MAPPING_TRUE)
    {
        m_hMapHandle = CreateFileMapping(m_hFileHandle, NULL, m_mode, 0, 0, NULL);
    }

    if (!m_hMapHandle)
    {
        CloseHandle(m_hFileHandle);
        m_hFileHandle = INVALID_HANDLE_VALUE;
        FIRTEX_THROW(FileIOException, "Cannot map file into shared memory: [%s]", m_sFileName.c_str());
    }

    map();
}

void MMapFileImpl::map()
{
    DWORD access = FILE_MAP_READ;
    if (m_mode == PAGE_READWRITE)
        access = FILE_MAP_WRITE;
    LPVOID addr = MapViewOfFile(m_hMapHandle, access, 0, 0, (SIZE_T)m_nFileSize);
    if (!addr)
    {
        FIRTEX_THROW_AND_LOG(FileIOException, "Cannot map shared memory object : [%s], "
                             "error code: [%d]", m_sFileName.c_str(), GetLastError());
    }

    m_pAddress = static_cast<char*>(addr);
}

void MMapFileImpl::unmap()
{
    if (m_pAddress)
    {
        UnmapViewOfFile(m_pAddress);
        m_pAddress = 0;
        return;
    }
}

void MMapFileImpl::close()
{
    if (m_hMapHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hMapHandle);
        m_hMapHandle = INVALID_HANDLE_VALUE;
    }

    if (m_hFileHandle != INVALID_HANDLE_VALUE && m_bHandleOwner == true)
    {
        CloseHandle(m_hFileHandle);
        m_hFileHandle = INVALID_HANDLE_VALUE;
    }
}

FX_NS_END

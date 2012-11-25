#ifndef __MMAPFILEIMPL_POSIX_H
#define __MMAPFILEIMPL_POSIX_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/MMapFile.h"
#include "firtex/common/RefCountedObject.h"
#include "firtex/common/IO.h"

FX_NS_DEF(utility);

/**
 * map file implementation for POSIX platforms.
 */ 
class MMapFileImpl : public FX_NS(common)::RefCountedObject
{
public:
    typedef FX_OS_FILE_DESCRIPTOR FileHandle;

public:
    MMapFileImpl();

public:
    /**
     * Creates or connects to a mapped file with the given name.
     * @param sFileName file name to map. 
     * @param mode file access mode, AM_READ for read AM_WRITE for write.
     * @param pAddrHit specifying the desired start address of the shared memory area. 
     *        Whether the hint is actually honored is, however, up to the system. Windows platform
     *        will generally ignore the hint.
     * @param bServer if false, the mapped file will be unlinked by calling shm_unlink (on POSIX platforms) 
     *        when the MMapFile object is destroyed. The bServer parameter is ignored on Windows platforms.
     */
    void open(const std::string& sFileName, std::size_t size,
              MMapFile::AccessMode mode, const void* addrHint, bool server);	

    /**
     * Maps the whole file into a shared memory segment.
     * @param file file object
     * @param pAddrHit specifying the desired start address of the shared memory area. 
     *        Whether the hint is actually honored is, however, up to the system. Windows platform
     *        will generally ignore the hint.
     */
    void open(const File& file, MMapFile::AccessMode mode, const void* addrHint);

    /**
     * Map a opened file handle
     *
     * @param fileHandle opened file handle
     * @param mode access mode
     * @param nMapSize number byte to map
     * @param pAddrHit specifying the desired start address of the shared memory area. 
     *        Whether the hint is actually honored is, however, up to the system. Windows platform
     *        will generally ignore the hint.
     */
    void open(const FileHandle& fileHandle, MMapFile::AccessMode mode,
              size_t nMapSize, const void* addrHint);	

public:
    /** 
     * Return the start address of the mapped file.
     * @return begin address of mapped file, will be NULL for illegal mapped file.
     */
    char* begin();
    const char* begin() const;

    /** 
     * Returns the one-past-end end address of the shared memory segment. 
     * @return end address of mapped file, will be NULL for illegal mapped file.
     */
    char* end();
    const char* end() const;
	
    /**
     * Return the file size 
     */
    int64_t length() const {return (int64_t)m_nSize;}

    /**
     * Return file name
     */
    std::string getFileName() const {return m_sFileName;}

protected:
    /** Maps the shared memory object.*/
    void map(const void* addrHint);

    /** Unmaps the shared memory object.*/
    void unmap();
		
    /** Releases the handle for the shared memory segment.*/
    void close();
		
    ~MMapFileImpl();

private:
    MMapFileImpl(const MMapFileImpl&);
    MMapFileImpl& operator = (const MMapFileImpl&);

    std::size_t m_nSize;
    
    int m_fd;
    char* m_pAddress;
    MMapFile::AccessMode m_access;
    std::string m_sFileName;
    bool m_bFileMapped;
    bool m_bServer;
    bool m_bHandleOwner;

private:
    DECLARE_STREAM_LOGGER();
};

///////////////////////////////////////////////////////
// inlines
//
inline char* MMapFileImpl::begin()
{
    return m_pAddress;
}

inline const char* MMapFileImpl::begin() const
{
    return m_pAddress;
}

inline char* MMapFileImpl::end()
{
    return m_pAddress + m_nSize;
}

inline const char* MMapFileImpl::end() const
{
    return m_pAddress + m_nSize;
}

FX_NS_END

#endif 

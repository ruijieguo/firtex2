#include "firtex/store/BlockFileOutputStream.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(store);

SETUP_STREAM_LOGGER(store, BlockFileOutputStream);

BlockFileOutputStream::BlockFileOutputStream(
        BlockFileSystem& fileSystem, const BlockFileSystem::FilePtr& file)
    : m_fileSystem(fileSystem)
    , m_pFile(file)
    , m_nBlockSize((uint32_t)fileSystem.getBlockSize())
    , m_nOffInBlk(0)
    , m_nCurPos(0)
{
    m_pBlockBuffer = new char[m_nBlockSize];
}

BlockFileOutputStream::~BlockFileOutputStream() 
{
    close();

    delete[] m_pBlockBuffer;
    m_pBlockBuffer = NULL;
}

void BlockFileOutputStream::write(const void* buffer, size_t bufSize)
{
    const char* data = (const char*)buffer;
    size_t bytesWritten = 0;

    blockid_t blockId = m_nCurPos / m_nBlockSize;
    if (m_nCurPos % m_nBlockSize != m_nOffInBlk)
    {
        FX_DEBUG("Read back block, block id: [%lld]", (long long int)blockId);
        m_fileSystem.readBlock(m_pFile, blockId, m_pBlockBuffer);
        m_nOffInBlk = (uint32_t)(m_nCurPos % m_nBlockSize);
    }

    // Step 1. write in current block buffer
    if (m_nOffInBlk < m_nBlockSize)
    {
        bytesWritten = bufSize > (m_nBlockSize - m_nOffInBlk) ? 
                    (m_nBlockSize - m_nOffInBlk) : bufSize;
        memcpy(m_pBlockBuffer + m_nOffInBlk, data, bytesWritten);
        bufSize -= bytesWritten;
        data += bytesWritten; 
        m_nOffInBlk += (uint32_t)bytesWritten;

        if (m_nOffInBlk == m_nBlockSize)
        {
            m_fileSystem.writeBlock(m_pFile, blockId++, m_pBlockBuffer);
            m_nOffInBlk = 0;
        }
    }

    // Step 2. write data to file system directly
    for (; (bufSize >= m_nBlockSize); blockId++)
    {
        m_fileSystem.writeBlock(m_pFile, blockId, data);
        data += m_nBlockSize;
        bufSize -= m_nBlockSize;
        bytesWritten += m_nBlockSize;
    }

    // Step 3. write the data less than a block
    while (bufSize > 0)
    {
        if (m_nOffInBlk == m_nBlockSize)
        {
            m_fileSystem.writeBlock(m_pFile, blockId++, m_pBlockBuffer);
            m_nOffInBlk = 0;
        }
        size_t bytesToWrite = bufSize > (m_nBlockSize - m_nOffInBlk) ? 
                    (m_nBlockSize - m_nOffInBlk) : bufSize;
        memcpy(m_pBlockBuffer + m_nOffInBlk, data, bytesToWrite);
        m_nOffInBlk += (uint32_t)bytesToWrite;
        bufSize -= bytesToWrite;
        bytesWritten += bytesToWrite;
    }
    m_nCurPos += (uint32_t)bytesWritten;
    updateFileSize();
}

void BlockFileOutputStream::write(InputStream& inStream, offset_t size)
{
    if (size == (offset_t)-1)
    {
        size = inStream.getSize() - inStream.tell();
    }
    offset_t nWriteSize = (m_nBlockSize - m_nOffInBlk);
    if (nWriteSize > size)
    {
        nWriteSize = size;
        inStream.read(m_pBlockBuffer + m_nOffInBlk, (size_t)nWriteSize);
        m_nOffInBlk += (uint32_t)nWriteSize;
        m_nCurPos += (uint32_t)nWriteSize;
    }
    else
    {
        inStream.read(m_pBlockBuffer + m_nOffInBlk, (size_t)nWriteSize);
        m_nOffInBlk += (uint32_t)nWriteSize;
        flushBlock();
        size -= nWriteSize;
        while (size > 0)
        {
            nWriteSize = (m_nBlockSize > size ) ? size : m_nBlockSize;
            inStream.read(m_pBlockBuffer, (size_t)nWriteSize);
            m_nOffInBlk = (uint32_t)nWriteSize;
            size -= nWriteSize;
            if (m_nOffInBlk == m_nBlockSize)
            {
                flushBlock();
            }
        }
        m_nCurPos += m_nOffInBlk;
    }
    updateFileSize();
}

void BlockFileOutputStream::copyTo(OutputStream& outStream)
{
    FIRTEX_THROW(UnsupportedException, _T("copyTo() operation is unsupported."));
}
    
void BlockFileOutputStream::flush()
{
    flushBlock();
}

void BlockFileOutputStream::seek(offset_t pos)
{
    if (pos < 0)
    {
        FIRTEX_THROW(FileIOException, "Invalid seek position.");
    }
    
    blockid_t blockId = m_nCurPos / m_nBlockSize;
    blockid_t newBlockId = pos / m_nBlockSize;

    if (blockId != newBlockId)
    {
        flushBlock();
        m_nOffInBlk = 0;
        m_nCurPos = pos;
    }
    else  // In current block
    {
        if (m_nOffInBlk != 0) // No data in buffer
        {
            m_nOffInBlk = (uint32_t)(pos % m_nBlockSize);
        }
        m_nCurPos = pos;
    }
    updateFileSize();
}

offset_t BlockFileOutputStream::tell() const
{
    return m_nCurPos;
}

void BlockFileOutputStream::close()
{
    if (m_pFile != NULL)
    {
        updateFileSize();
        flush();
        m_fileSystem.closeFile(m_pFile);
        m_pFile.reset();
    }
}

FX_NS_END


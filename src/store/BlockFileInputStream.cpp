#include "firtex/store/BlockFileInputStream.h"

FX_NS_DEF(store);

SETUP_STREAM_LOGGER(store, BlockFileInputStream);

BlockFileInputStream::BlockFileInputStream(
        BlockFileSystem& fileSystem, const BlockFileSystem::FilePtr& file) 
    : m_fileSystem(fileSystem)
    , m_pFile(file)
    , m_pBlockBuffer(NULL)
    , m_nBlockSize((uint32_t)fileSystem.getBlockSize())
    , m_nOffInBlk(m_nBlockSize)
    , m_nCurPos(0)
{
    m_pBlockBuffer = new char[m_nBlockSize];
}

BlockFileInputStream::BlockFileInputStream(const BlockFileInputStream& src) 
    : m_fileSystem(src.m_fileSystem)
    , m_pBlockBuffer(NULL)
    , m_nBlockSize(src.m_nBlockSize)
    , m_nOffInBlk(m_nBlockSize)
    , m_nCurPos(0)
{
    m_pFile = m_fileSystem.doOpenFile(src.m_pFile->fileName);
    m_pBlockBuffer = new char[m_nBlockSize];
}

BlockFileInputStream::~BlockFileInputStream() 
{
    close();

    delete[] m_pBlockBuffer;
    m_pBlockBuffer = NULL;
}

size_t BlockFileInputStream::read(void* buffer, size_t bufSize)
{
    char* data = (char*)buffer;
    size_t bytesRead = 0;
    if (m_nCurPos >= m_pFile->fileActualSize)
        return 0;

    if ((offset_t)bufSize > (m_pFile->fileActualSize - m_nCurPos))
    {
        bufSize = (size_t)(m_pFile->fileActualSize - m_nCurPos);
    }

    blockid_t blockId = m_nCurPos / m_nBlockSize;

    if (m_nCurPos % m_nBlockSize != m_nOffInBlk)
    {
        m_fileSystem.readBlock(m_pFile, blockId, m_pBlockBuffer);
        m_nOffInBlk = (uint32_t)(m_nCurPos % m_nBlockSize);
    }

    // Step 1. read in current block buffer
    if (m_nOffInBlk < m_nBlockSize)
    {
        bytesRead = bufSize > (m_nBlockSize - m_nOffInBlk) ? 
                    (m_nBlockSize - m_nOffInBlk) : bufSize;
        memcpy(data, m_pBlockBuffer + m_nOffInBlk, bytesRead);
        bufSize -= bytesRead;
        data += bytesRead; 
        m_nOffInBlk += (uint32_t)bytesRead;
        ++blockId;
    }

    // Step 2. read data from file system directly
    for (; (bufSize >= m_nBlockSize); blockId++)
    {
        m_fileSystem.readBlock(m_pFile, blockId, data);
        data += m_nBlockSize;
        bufSize -= m_nBlockSize;
        bytesRead += m_nBlockSize;
    }

    // Step 3. read the data less than a block
    if (bufSize > 0)
    {
        m_fileSystem.readBlock(m_pFile, blockId, m_pBlockBuffer);
        memcpy(data, m_pBlockBuffer, bufSize);
        m_nOffInBlk = (uint32_t)bufSize;
        bytesRead += bufSize;
    }
    m_nCurPos += (uint32_t)bytesRead;
    return bytesRead;
}

size_t BlockFileInputStream::readMayCopy(void*& buffer, size_t bufSize)
{
    if (m_nCurPos >= m_pFile->fileActualSize)
        return 0;

    if ((offset_t)bufSize > (m_pFile->fileActualSize - m_nCurPos))
    {
        bufSize = (size_t)(m_pFile->fileActualSize - m_nCurPos);
    }

    if (m_nCurPos % m_nBlockSize != m_nOffInBlk)
    {
        blockid_t blockId = m_nCurPos / m_nBlockSize;
        m_fileSystem.readBlock(m_pFile, blockId, m_pBlockBuffer);
        m_nOffInBlk = (uint32_t)(m_nCurPos % m_nBlockSize);
    }

    if ((size_t)m_nOffInBlk + bufSize < (size_t)m_nBlockSize)
    {
        buffer = m_pBlockBuffer + m_nOffInBlk;
        m_nOffInBlk += (uint32_t)bufSize;
        return bufSize;
    }

    return read(buffer, bufSize);
}

void BlockFileInputStream::seek(offset_t pos)
{
    if (pos > m_pFile->fileActualSize || pos < 0)
    {
        FIRTEX_THROW(FileIOException, "Seek past EOF: "
                     "[pos=%lld, file size=%lld], file=[%s]", (long long int)pos, 
                     (long long int)m_pFile->fileActualSize, m_pFile->fileName.c_str());
    }

    blockid_t blockId = m_nCurPos / m_nBlockSize;
    blockid_t newBlockId = pos / m_nBlockSize;
    if (blockId != newBlockId)
    {
        m_nOffInBlk = m_nBlockSize;
        m_nCurPos = pos;
    }
    else // In current block buffer
    {
        if ((m_nCurPos % m_nBlockSize) == m_nOffInBlk)
        {
            m_nOffInBlk = (uint32_t)(pos % m_nBlockSize);
        }
        else // No data in buffer
        {
            m_nOffInBlk = m_nBlockSize;
        }
        m_nCurPos = pos;
    }
}

size_t BlockFileInputStream::seekAndRead(offset_t pos, void* buffer, size_t bufSize)
{
    seek(pos);
    return read(buffer, bufSize);
}

offset_t BlockFileInputStream::tell() const
{
    return m_nCurPos;
}

offset_t BlockFileInputStream::getSize() const
{
    return m_pFile->fileActualSize;
}

std::string BlockFileInputStream::getFilePath() const
{
    return m_fileSystem.getPath() + m_pFile->fileName;
}

bool BlockFileInputStream::isEof() const
{
    return (m_nCurPos == m_pFile->fileActualSize);
}

void BlockFileInputStream::close()
{
    if (m_pFile.isNotNull())
    {
        m_fileSystem.closeFile(m_pFile);
        m_pFile.reset();
    }
}

InputStream* BlockFileInputStream::clone() const
{
    return new BlockFileInputStream(*this);
}

FX_NS_END


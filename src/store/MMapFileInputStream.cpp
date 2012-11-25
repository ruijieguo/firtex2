#include "firtex/store/MMapFileInputStream.h"

FX_NS_USE(utility);

FX_NS_DEF(store);

SETUP_STREAM_LOGGER(store, MMapFileInputStream);

MMapFileInputStream::MMapFileInputStream(MMapFileSystem& fileSystem,
        const BlockFileSystem::FilePtr& pFile, const MMapFilePtr& pMMapFile)
    : m_fileSystem(fileSystem)
    , m_pFile(pFile)
    , m_pMMapFile(pMMapFile)
    , m_nCurPos(0)
{
    m_pBegin = m_pMMapFile->begin();
    m_pEnd = m_pMMapFile->end();
}

MMapFileInputStream::MMapFileInputStream(const MMapFileInputStream& src)
    : m_fileSystem(src.m_fileSystem)
    , m_nCurPos(0)
{
    m_pFile = src.m_pFile;
    m_pMMapFile = src.m_pMMapFile;
    m_pBegin = m_pMMapFile->begin();
    m_pEnd = m_pMMapFile->end();
}

MMapFileInputStream::~MMapFileInputStream() 
{
    close();
}

size_t MMapFileInputStream::read(void* buffer, size_t bufSize)
{
    if ((offset_t)bufSize > ((offset_t)(m_pEnd - m_pBegin) - m_nCurPos))
    {
        bufSize = (size_t)((offset_t)(m_pEnd - m_pBegin) - m_nCurPos);
    }

    memcpy(buffer, m_pBegin + m_nCurPos, bufSize);
    m_nCurPos += (offset_t)bufSize;
    return bufSize;
}

size_t MMapFileInputStream::readMayCopy(void*& buffer, size_t bufSize)
{
    if ((offset_t)bufSize > ((offset_t)(m_pEnd - m_pBegin) - m_nCurPos))
    {
        bufSize = (size_t)((offset_t)(m_pEnd - m_pBegin) - m_nCurPos);
    }

    buffer = m_pBegin + m_nCurPos;
    m_nCurPos += (offset_t)bufSize;
    return bufSize;
}

void MMapFileInputStream::seek(offset_t pos)
{
    if (pos < 0 || pos > m_pFile->fileActualSize)
    {
        FIRTEX_THROW(FileIOException, "Seek out of range, pos: [%lld], "
                     "file size: [%lld]", (long long int)pos, 
                     (long long int)m_pFile->fileActualSize);
    }

    m_nCurPos = pos;
}

size_t MMapFileInputStream::seekAndRead(offset_t pos, void* buffer, size_t bufSize)
{
    seek(pos);
    return read(buffer, bufSize);
}

offset_t MMapFileInputStream::tell() const
{
    return m_nCurPos;
}

offset_t MMapFileInputStream::getSize() const
{
    return m_pFile->fileActualSize;
}

std::string MMapFileInputStream::getFilePath() const
{
    return m_fileSystem.getPath() + m_pFile->fileName;
}

bool MMapFileInputStream::isEof() const
{
    return (m_nCurPos == m_pFile->fileActualSize);
}

void MMapFileInputStream::close()
{
    if (m_pMMapFile.isNotNull())
    {
        m_pMMapFile.reset();
    }

    if (m_pFile.isNotNull())
    {
        m_fileSystem.closeFile(m_pFile);
        m_pFile.reset();
    }
}

InputStream* MMapFileInputStream::clone() const
{
    return new MMapFileInputStream(*this);
}

FX_NS_END


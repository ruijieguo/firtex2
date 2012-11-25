#include "firtex/index/ByteSliceInputStream.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, ByteSliceInputStream);

ByteSliceInputStream::ByteSliceInputStream(ByteSlice* pHeadSlice, bool bDestroySlice, size_t nSize)
    : m_pHeadSlice(pHeadSlice)
    , m_nTotalSize(nSize)
    , m_pCurSlice(pHeadSlice)
    , m_nCurPosInSlice(0)
    , m_nCurPos(0)
    , m_bDestroySlice(bDestroySlice)
{
    if (m_nTotalSize == (size_t)(-1))
    {
        m_nTotalSize = 0;
        ByteSlice* pSlice = m_pHeadSlice;
        while (pSlice)
        {
            m_nTotalSize += pSlice->size;
            pSlice = pSlice->next;
        }
    }
}

ByteSliceInputStream::ByteSliceInputStream(const ByteSliceInputStream& src)
    : m_pHeadSlice(src.m_pHeadSlice)
    , m_nTotalSize(src.m_nTotalSize)
    , m_pCurSlice(src.m_pCurSlice)
    , m_nCurPosInSlice(src.m_nCurPosInSlice)
    , m_nCurPos(src.m_nCurPos)
    , m_bDestroySlice(false)
{
}

ByteSliceInputStream::~ByteSliceInputStream() 
{
    close();
}

size_t ByteSliceInputStream::read(void* buffer, size_t bufSize)
{
    uint8_t* pBuf = (uint8_t*)buffer;
    size_t nBytesRead = bufSize;
    while (nBytesRead > 0)
    {
        if (!m_pCurSlice)
        {
            FIRTEX_THROW(FileIOException, "Read past EOF.");
        }
        size_t nBytesToRead = m_pCurSlice->size - m_nCurPosInSlice;
        if (nBytesToRead > nBytesRead)
        {
            nBytesToRead = nBytesRead;
            memcpy(pBuf, m_pCurSlice->data + m_nCurPosInSlice, nBytesToRead);
            m_nCurPosInSlice += (uint32_t)nBytesToRead;
        }
        else 
        {
            memcpy(pBuf, m_pCurSlice->data + m_nCurPosInSlice, nBytesToRead);
            m_pCurSlice = m_pCurSlice->next;
            m_nCurPosInSlice = 0;
        }
        pBuf += nBytesToRead;
        nBytesRead -= nBytesToRead;
        m_nCurPos += (offset_t)nBytesToRead;
    }
    return bufSize;
}

size_t ByteSliceInputStream::readMayCopy(void*& buffer, size_t bufSize)
{
    size_t nBytesRead = bufSize;
    size_t nBytesLeft = m_pCurSlice->size - m_nCurPosInSlice;
    if (nBytesLeft >= nBytesRead)
    {
        buffer = m_pCurSlice->data + m_nCurPosInSlice;
        m_nCurPosInSlice += (uint32_t)nBytesRead;
        m_nCurPos += (offset_t)nBytesRead;
        return nBytesRead;
    }
    return read(buffer, bufSize);
}

void ByteSliceInputStream::seek(offset_t pos)
{
    if (pos < m_nCurPos)
    {
        m_nCurPos = 0;
        m_pCurSlice = m_pHeadSlice;
        m_nCurPosInSlice = 0;
    }
    while (pos > m_nCurPos)
    {
        offset_t nDelt = pos - m_nCurPos;

        if (!m_pCurSlice)
        {
            FIRTEX_THROW(FileIOException, "Seek past EOF.");
        }
        uint32_t nBytesToSeek = (uint32_t)(m_pCurSlice->size) - m_nCurPosInSlice;
        if ((offset_t)nBytesToSeek > nDelt)
        {
            nBytesToSeek = (uint32_t)nDelt;
            m_nCurPosInSlice += nBytesToSeek;
        }
        else 
        {
            m_pCurSlice = m_pCurSlice->next;
            m_nCurPosInSlice = 0;
        }
        m_nCurPos += nBytesToSeek;
    }
}

size_t ByteSliceInputStream::seekAndRead(offset_t pos, void* buffer, size_t bufSize)
{
    seek(pos);
    return read(buffer, bufSize);
}

offset_t ByteSliceInputStream::tell() const
{
    return m_nCurPos;
}

offset_t ByteSliceInputStream::getSize() const
{
    return (offset_t)m_nTotalSize;
}

std::string ByteSliceInputStream::getFilePath() const
{
    static std::string EMPTY_STR;
    return EMPTY_STR;
}

bool ByteSliceInputStream::isEof() const
{
    return (m_nCurPos >= (offset_t)m_nTotalSize);
}

void ByteSliceInputStream::close()
{
    if (m_bDestroySlice)
    {
        ByteSlice* pSlice = m_pHeadSlice;
        while (pSlice)
        {
            ByteSlice* pTmp = pSlice;
            pSlice = pSlice->next;
            ByteSlice::destroy(pTmp);
        }
    }
    m_pHeadSlice = NULL;
    m_pCurSlice = NULL;
    m_nTotalSize = 0;
    m_nCurPos = 0;
    m_nCurPosInSlice = 0;
}

FX_NS(store)::InputStream* ByteSliceInputStream::clone() const
{
    return new ByteSliceInputStream(*this);
}

FX_NS_END


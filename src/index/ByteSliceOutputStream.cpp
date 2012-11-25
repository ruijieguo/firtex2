#include "firtex/index/ByteSliceOutputStream.h"
#include "firtex/utility/Exception.h"

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, ByteSliceOutputStream);

size_t ByteSliceOutputStream::ALLOC_STRATEGY = ByteSliceOutputStream::ALLOC_EXPLIMIT;
size_t ByteSliceOutputStream::LOWER_LIMIT = 24;
size_t ByteSliceOutputStream::UPPER_LIMIT = 524288;///512K
float ByteSliceOutputStream::INC_RATIO = 1.5;

ByteSliceOutputStream::ByteSliceOutputStream(Pool* pPool)
    : m_pHeadSlice(NULL)
    , m_pTailSlice(NULL)
    , m_nTotalSize(0)
    , m_pWriteUpto(NULL)
    , m_pPool(pPool)
{
    newSlice();
}		

ByteSliceOutputStream::~ByteSliceOutputStream()
{
}

void ByteSliceOutputStream::reset()
{
    m_pHeadSlice = m_pTailSlice = NULL;
    m_pWriteUpto = NULL;
    m_nTotalSize = 0;
}

void ByteSliceOutputStream::copyTo(OutputStream& outStream)
{
    if(m_pHeadSlice)
    {
        ByteSlice* pSlice = m_pHeadSlice;
        while (pSlice && pSlice != m_pTailSlice)
        {
            outStream.write((const void*)pSlice->data, pSlice->size);
            pSlice = pSlice->next;
        }
        outStream.write((const void*)pSlice->data, m_pWriteUpto - pSlice->data);
    }
}

void ByteSliceOutputStream::write(const void* buffer, size_t bufSize)
{
    const char* pBufferPtr = (const char*)buffer;
    while (bufSize > 0)
    {
        if (!m_pWriteUpto || m_pWriteUpto >= m_pTailSlice->data + m_pTailSlice->size)
        {
            newSlice();
        }
        size_t nWriteSize = (bufSize > (size_t)(m_pTailSlice->data + m_pTailSlice->size - m_pWriteUpto)) ?
                            (m_pTailSlice->data + m_pTailSlice->size - m_pWriteUpto) : bufSize;
        memcpy(m_pWriteUpto, pBufferPtr, nWriteSize);
        m_pWriteUpto += nWriteSize;
        pBufferPtr += nWriteSize;
        bufSize -= nWriteSize;
    }
}

void ByteSliceOutputStream::write(InputStream& inStream, offset_t size)
{
    offset_t sizeToCopy = size;
    if (size == (offset_t)(-1))
    {
        sizeToCopy = inStream.getSize() - inStream.tell();
    }
    while (sizeToCopy > 0)
    {
        if (!m_pWriteUpto || m_pWriteUpto >= m_pTailSlice->data + m_pTailSlice->size)
        {
            newSlice();
        }
        size_t nWriteSize = ((size_t)sizeToCopy > (size_t)(m_pTailSlice->data + m_pTailSlice->size - m_pWriteUpto)) ?
                            (m_pTailSlice->data + m_pTailSlice->size - m_pWriteUpto) : (size_t)sizeToCopy;
        inStream.read((void*)m_pWriteUpto, nWriteSize);
        m_pWriteUpto += nWriteSize;
        sizeToCopy -= (offset_t)nWriteSize;
    }
}

void ByteSliceOutputStream::newSlice()
{
    size_t nNewSize = getSliceSize(m_nTotalSize);
    void* begin = m_pPool->tryAllocate(nNewSize);
    ///allocate memory failed, decrease chunk size
    if(!begin)
    {
        ///Decrease allocate size
        nNewSize = DEFAULT_EXCEPTION_ALLOC_SIZE;
        begin = m_pPool->allocate(nNewSize);
        FIRTEX_ASSERT2(begin);
    }

    ByteSlice* pSlice = (ByteSlice*)begin;
    pSlice->size = nNewSize - sizeof(ByteSlice) + sizeof(uint8_t);
    pSlice->next = NULL;

    if(!m_pTailSlice)
    {
        m_pTailSlice = m_pHeadSlice = pSlice;
    }
    else
    {
        m_pTailSlice->next = pSlice;
        m_nTotalSize += m_pTailSlice->size;
        m_pTailSlice = pSlice;				
    }
    m_pWriteUpto = m_pTailSlice->data;		
}

void ByteSliceOutputStream::flush()
{
}

void ByteSliceOutputStream::seek(offset_t pos)
{
    FIRTEX_THROW(UnsupportedException, "Seek operation is unsupported.");
}

offset_t ByteSliceOutputStream::tell() const
{
    if (!m_pTailSlice)
        return 0;
    return (offset_t)(m_nTotalSize + (m_pWriteUpto - m_pTailSlice->data));
}

void ByteSliceOutputStream::close()
{
}

FX_NS_END


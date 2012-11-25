#include "firtex/extension/processor/GZipDocumentReader.h"
#include "zlib/zlib.h"

FX_NS_USE(utility);

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(processor, GZipDocumentReader);

#pragma pack(push,1)
struct GZipHeader
{
    unsigned char magic1; // 0x1f
    unsigned char magic2; // 0x8b
    unsigned char method; // 8
    unsigned char flags;
    uint32_t modTime;
    unsigned char extraFlags;
    unsigned char os;
};
#pragma pack(pop)

/* gzip flag byte */
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text */
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define RESERVED     0xE0 /* bits 5..7: reserved */

GZipDocumentReader::GZipDocumentReader(size_t nBuffSize) 
    : m_nFileLength(0)
    , m_pZStream(NULL)
    , m_nBuffSize(nBuffSize)
    , m_nBytesInBuffer(0)
    , m_nReadUpTo(0)
{
    if (m_nBuffSize == 0)
    {
        m_nBuffSize = DEFAULT_BUFFER_SIZE;
    }
        
    m_pBuffer = new char[m_nBuffSize];
}

GZipDocumentReader::~GZipDocumentReader() 
{
    delete[] m_pBuffer;
    m_pBuffer = NULL;

    /* clean up and return */
    int ret = inflateEnd(m_pZStream);
    if (ret != Z_OK)
    {
        FX_LOG(ERROR, "Close gzip stream FAILED: [%s].", getFilePath().c_str());
    }
				
    if (m_pZStream)
    {
        delete m_pZStream;
        m_pZStream = NULL;
    }
}

void GZipDocumentReader::open(const std::string& sFilePath)
{
    m_file.open(sFilePath, BinaryFile::READ);
    m_nFileLength = m_file.getLength();
    
    /* allocate inflate state */
    FIRTEX_ASSERT2(!m_pZStream);
    m_pZStream = new z_stream;
    m_pZStream->zalloc = Z_NULL;
    m_pZStream->zfree = Z_NULL;
    m_pZStream->opaque = Z_NULL;
    m_pZStream->avail_in = 0;
    m_pZStream->next_in = Z_NULL;
    int ret = inflateInit2(m_pZStream, -MAX_WBITS);
    if (ret != Z_OK)
    {		
        FIRTEX_THROW(FileIOException, "Decompress GZip file FAILED: [%s].", sFilePath.c_str());
    }
    
    GZipHeader header;
    m_file.read(&header, sizeof(GZipHeader));

    if (!refillBuffer())
    {
        FIRTEX_THROW(FileIOException, "Refill buffer FAILED: [%s].", sFilePath.c_str());
    }

    if (!checkHeader(header))
    {
        FIRTEX_THROW(FileIOException, "Check GZip header FAILED: [%s].", sFilePath.c_str());
    }
}

bool GZipDocumentReader::isEof()
{
    return (m_file.getPos() >= m_nFileLength) && (m_nBytesInBuffer == m_nReadUpTo);
}

size_t GZipDocumentReader::read(void* pData, size_t nBytes)
{
    size_t nBytesRead = 0;

    m_pZStream->avail_out = (uInt)nBytes;
    m_pZStream->next_out = (Bytef*)pData;

    do 
    {
        if (m_nReadUpTo >= m_nBytesInBuffer) 
        {
            if (!refillBuffer())
            {
                break;
            }
        }
        
        m_pZStream->avail_in = (uInt)(m_nBytesInBuffer - m_nReadUpTo);
        m_pZStream->next_in = (Bytef*)(m_pBuffer + m_nReadUpTo);

        int ret = inflate(m_pZStream, Z_NO_FLUSH);
        if ((ret != Z_STREAM_END) && (ret != Z_OK)) 
        {
            FX_LOG(WARN, "zlib infalte error [%d]", ret);
            return 0;
        }

        m_nReadUpTo = m_nBytesInBuffer - m_pZStream->avail_in;
        if (ret == Z_STREAM_END) 
        {
            // skip rest data
            while (refillBuffer());
            break;
        }
    } while (m_pZStream->avail_out > 0);

    nBytesRead = nBytes - m_pZStream->avail_out;
    return nBytesRead;
}

int64_t GZipDocumentReader::tell()
{
    return m_file.getPos() - (m_nBytesInBuffer - m_nReadUpTo);
}

std::string GZipDocumentReader::getFilePath() const
{
    return m_file.getFileName();
}

bool GZipDocumentReader::checkHeader(const GZipHeader& header)
{
    if ((header.magic1 != 0x1f) ||
        (header.magic2 != 0x8b) ||
        (header.method != Z_DEFLATED) ||
        (header.flags & RESERVED)) 
    {
        return false;
    }

    if(header.flags & EXTRA_FIELD) 
    {
        char buf[2];
        if (!getBytes(buf, sizeof(buf)))
        {
            return false;
        }

        unsigned int len = (unsigned)(buf[1]);
        len <<= 8;
        len += (unsigned)(buf[0]);
        while (len--) 
        {
            char check[1];
            if (!getBytes(check, sizeof(check)))
            {
                return false;
            }
            if (check[0] < 0) 
            {
                break;
            }
        }
    }

    if(header.flags & ORIG_NAME) 
    {
        char check[1];
        do 
        {
            if (!getBytes(check, sizeof(check)))
            {
                return false;
            }
        } while (check[0] > 0);
    }
    
    if (header.flags & COMMENT)
    {
        char check[1];
        do
        {
            if (!getBytes(check, sizeof(check)))
            {
                return false;
            }
        } while (check[0] > 0);
    }

    if (header.flags & HEAD_CRC)
    {
        char check[2];
        if (!getBytes(check, sizeof(check))) 
        {
            return false;
        }
    }
    return true;
}

FX_NS_END


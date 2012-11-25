#include "firtex/extension/processor/PlainDocumentReader.h"

FX_NS_USE(utility);

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(processor, PlainDocumentReader);

PlainDocumentReader::PlainDocumentReader() 
    : m_nFileLength(0)
{
}

PlainDocumentReader::~PlainDocumentReader() 
{
}

void PlainDocumentReader::open(const std::string& sFilePath)
{
    m_file.open(sFilePath, BinaryFile::READ);
    m_nFileLength = m_file.getLength();
}

bool PlainDocumentReader::isEof()
{
    return (m_file.getPos() >= m_nFileLength);
}

size_t PlainDocumentReader::read(void* pData, size_t nBytes)
{
    return m_file.read(pData, nBytes);
}

int64_t PlainDocumentReader::tell()
{
    return m_file.getPos();
}

std::string PlainDocumentReader::getFilePath() const
{
    return m_file.getFileName();
}


FX_NS_END


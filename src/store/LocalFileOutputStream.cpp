#include "firtex/store/LocalFileOutputStream.h"

FX_NS_USE(utility);

FX_NS_DEF(store);

LocalFileOutputStream::LocalFileOutputStream() 
{
}

LocalFileOutputStream::~LocalFileOutputStream() 
{
    close();
}

void LocalFileOutputStream::open(const std::string& sFileName, OpenMode om)
{
    m_fileWriter.open(sFileName, om == APPEND ? BinaryFile::RDWR : BinaryFile::CRTR);
}

void LocalFileOutputStream::write(const void* buffer, size_t bufSize)
{
    m_fileWriter.write(buffer, bufSize);
}
    
void LocalFileOutputStream::write(InputStream& inStream, offset_t size)
{
    const static size_t BUFFER_SIZE = 204;
    char buffer[BUFFER_SIZE];
    if (size == (offset_t)-1)
    {
        size = inStream.getSize();
    }
    while (size > 0)
    {
        size_t nWriteUpTo = (size > (offset_t)BUFFER_SIZE) ? BUFFER_SIZE : (size_t)size;
        inStream.read(buffer, nWriteUpTo);
        write(buffer, nWriteUpTo);
        size -= nWriteUpTo;
    }
}

void LocalFileOutputStream::writeString(const std::string& str)
{
    write(str.c_str(), str.length());
}

void LocalFileOutputStream::copyTo(OutputStream& outStream)
{
    FIRTEX_THROW(UnsupportedException, _T("copyTo() operation is unsupported."));
}
    
void LocalFileOutputStream::flush()
{
    m_fileWriter.flush();
}

void LocalFileOutputStream::seek(offset_t pos)
{
    m_fileWriter.seek((int64_t)pos);
}

offset_t LocalFileOutputStream::tell() const
{
    return (offset_t)m_fileWriter.getPos();
}

void LocalFileOutputStream::close()
{
    m_fileWriter.close();
}

FX_NS_END


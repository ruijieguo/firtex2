#include "firtex/utility/XMLDocumentWrapper.h"
#include "firtex/utility/BinaryFile.h"

using namespace std;
FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, XMLDocumentWrapper);

XMLDocumentWrapper::XMLDocumentWrapper() 
{
    m_pNode = &m_doc;
}

XMLDocumentWrapper::~XMLDocumentWrapper() 
{
}

void XMLDocumentWrapper::parse(const std::string& sXML)
{
    try 
    {
        m_doc.parse<rapidxml::parse_full>(m_doc.allocate_string(sXML.c_str()));
    }
    catch(rapidxml::parse_error& e)
    {
        FIRTEX_THROW(BadXmlFormatException, "Bad xml format: [%s]", e.what());
    }
}

void XMLDocumentWrapper::parseFile(const std::string& sFile)
{
    BinaryFile bf;
    bf.open(sFile, BinaryFile::READ);

    size_t len = (size_t)bf.getLength();
    char* buf = m_doc.allocate_string(NULL, len + 1);
    buf[len] = 0;
    try
    {
        bf.read(buf, len);
    }
    catch (const FileIOException& fe)
    {
        bf.close();
        clear();
        FIRTEX_RETHROW(fe);
    }	
    bf.close();

    try 
    {
        m_doc.parse<rapidxml::parse_full>(buf);
    }
    catch(rapidxml::parse_error& e)
    {
        clear();
        FIRTEX_THROW(BadXmlFormatException, "Bad xml format: [%s]", e.what());
    }
}

void XMLDocumentWrapper::clear()
{
    try 
    {
        m_doc.clear();
    }
    catch(rapidxml::parse_error& e)
    {
        FIRTEX_THROW(BadXmlFormatException, "Bad xml format: [%s]", e.what());
    }
}

void XMLDocumentWrapper::print(std::basic_ostream<char>& out)
{
    try
    {
        rapidxml::print<char>(out, m_doc);
    }
    catch(rapidxml::parse_error& e)
    {
        FIRTEX_THROW(BadXmlFormatException, "Bad xml format: [%s]",
                     e.what());
    }
}

void XMLDocumentWrapper::print(std::string& out)
{
    try
    {
        rapidxml::print(std::back_inserter(out), m_doc);
    }
    catch(rapidxml::parse_error& e)
    {
        FIRTEX_THROW(BadXmlFormatException, "Bad xml format: [%s]",
                     e.what());
    }
}

void XMLDocumentWrapper::printToFile(std::string& sFile)
{
    ostringstream os;
    print(os);
    BinaryFile bf;
    bf.open(sFile, BinaryFile::CREATE);
    bf.write(os.str().c_str(), os.str().length());
}

FX_NS_END


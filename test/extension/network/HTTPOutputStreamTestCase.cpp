#include "HTTPOutputStreamTestCase.h"

using namespace std;

FX_NS_DEF(network);

SETUP_STREAM_LOGGER(network, HTTPOutputStreamTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(HTTPOutputStreamTestCase);

HTTPOutputStreamTestCase::HTTPOutputStreamTestCase() 
{
}

HTTPOutputStreamTestCase::~HTTPOutputStreamTestCase() 
{
}

void HTTPOutputStreamTestCase::setUp()
{
}

void HTTPOutputStreamTestCase::tearDown()
{
}

void HTTPOutputStreamTestCase::testWriteData()
{
    TCPConnectionPtr dummyConn;
    HTTPOutputStreamTestCase outStream;

    std::vector<std::string> answer;
    
    size_t intVal = 10;
    outStream.write(intVal);
    answer.push_back("10");
    
    string binData = "bin";
    outStream.write(binData.c_str(), binData.length());
    answer.push_back(binData);

    binData.clear();

    string noCpStr = "nocopy";
    outStream.writeNoCopy(noCpStr);
    answer.push_back(noCpStr);

    string noCpData = "NoCpBin";
    outStream.writeNoCopy((void*)noCpData.c_str(), noCpData.length());
    answer.push_back(noCpData);

    HTTPOutputStream::ConstBuffers buffers;
    outStream.toBuffers(buffers, false);

    CPPUNIT_ASSERT_EQUAL(answer.size(), buffers.size());

    for (size_t i = 0; i < buffers.size(); ++i)
    {
        //std::cout << (const char*)asio::detail::buffer_cast_helper(*it);
        string str = (const char*)asio::detail::buffer_cast_helper(buffers[i]);
        CPPUNIT_ASSERT_EQUAL(answer[i], str);
    }
}

FX_NS_END


#ifndef __FX_HTTPOUTPUTSTREAMTESTCASE_H
#define __FX_HTTPOUTPUTSTREAMTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/extension/network/HTTPOutputStream.h"
#include "firtex/extension/network/HTTPRequest.h"


FX_NS_DEF(network);

class HTTPOutputStreamTestCase : public CPPUNIT_NS::TestFixture, public HTTPOutputStream
{
    CPPUNIT_TEST_SUITE(HTTPOutputStreamTestCase);
    CPPUNIT_TEST(testWriteData);
    CPPUNIT_TEST_SUITE_END();
public:
    HTTPOutputStreamTestCase();
    ~HTTPOutputStreamTestCase();

    virtual HTTPMessage& getMessage() 
    {
        static HTTPRequest DUMMY_REQ;
        return DUMMY_REQ;
    };

    virtual void headerToBuffers(ConstBuffers& buffers) {}

public:
    void setUp();
    void tearDown();

protected:
    void testWriteData();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_HTTPOUTPUTSTREAMTESTCASE_H

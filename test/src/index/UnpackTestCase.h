#ifndef __FX_UNPACKTESTCASE_H
#define __FX_UNPACKTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include <vector>

FX_NS_DEF(index);

class UnpackTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(UnpackTestCase);
    CPPUNIT_TEST(testUnpack);
    CPPUNIT_TEST_SUITE_END();
public:
    UnpackTestCase();
    ~UnpackTestCase();

    void setUp();
    void tearDown();

protected:
    void testUnpack();

    void createData(std::vector<uint32_t>& data, size_t n, uint64_t nMaxValue);
private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_UNPACKTESTCASE_H

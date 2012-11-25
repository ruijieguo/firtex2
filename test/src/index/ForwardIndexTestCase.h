#ifndef __FX_FORWARDINDEXTESTCASE_H
#define __FX_FORWARDINDEXTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/index/ForwardIndexIterator.h"
#include <vector>
#include <string>

FX_NS_DEF(index);

class ForwardIndexTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ForwardIndexTestCase);
    CPPUNIT_TEST(testWriteAndRead);
    CPPUNIT_TEST(testWriteAndReadWithMultiBarrels);
    CPPUNIT_TEST_SUITE_END();
public:
    ForwardIndexTestCase();
    ~ForwardIndexTestCase();

    void setUp();
    void tearDown();

protected:
    void testWriteAndRead();
    void testWriteAndReadWithMultiBarrels();

protected:
    std::string getIndexPath();
    void buildForwardIndex(const std::string& sDocs);

    template<typename T>
    void checkForwardIndex(const ForwardIndexIteratorPtr& pIter,
                           const std::vector<T>& answer);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_FORWARDINDEXTESTCASE_H

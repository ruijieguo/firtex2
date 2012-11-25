#ifndef __FX_FIELDSELECTCLAUSETESTCASE_H
#define __FX_FIELDSELECTCLAUSETESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(search);

class FieldSelectClauseTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(FieldSelectClauseTestCase);
    CPPUNIT_TEST(testFromString);
    CPPUNIT_TEST_SUITE_END();
public:
    FieldSelectClauseTestCase();
    ~FieldSelectClauseTestCase();

    void setUp();
    void tearDown();

protected:
    void testFromString();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_FIELDSELECTCLAUSETESTCASE_H

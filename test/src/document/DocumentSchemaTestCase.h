#ifndef __FX_DOCUMENTSCHEMATESTCASE_H
#define __FX_DOCUMENTSCHEMATESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(document);

class DocumentSchemaTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(DocumentSchemaTestCase);
    CPPUNIT_TEST(testReadSchema);
    CPPUNIT_TEST_SUITE_END();
public:
    DocumentSchemaTestCase();
    ~DocumentSchemaTestCase();

    void setUp();
    void tearDown();

protected:
    void testReadSchema();

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DOCUMENTSCHEMATESTCASE_H

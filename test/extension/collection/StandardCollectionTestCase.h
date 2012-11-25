#ifndef __FX_STANDARDCOLLECTIONTESTCASE_H
#define __FX_STANDARDCOLLECTIONTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/extension/collection/CollectionConfigure.h"

FX_NS_DEF(collection);

class StandardCollectionTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(StandardCollectionTestCase);
    CPPUNIT_TEST(testScan);
    CPPUNIT_TEST(testScanAsync);
    CPPUNIT_TEST(testScanWithMonitor);
    CPPUNIT_TEST_SUITE_END();
public:
    StandardCollectionTestCase();
    ~StandardCollectionTestCase();

    void setUp();
    void tearDown();

protected:
    void testScan();
    void testScanAsync();
    void testScanWithMonitor();

private:
	std::string getFilePath(const std::string& sDirName, const std::string& sFileName = "");

private:
    void checkIndex();
private:
    CollectionConfigurePtr m_pConf;
    FX_NS(document)::DocumentSchemaPtr m_pDocSchema;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_STANDARDCOLLECTIONTESTCASE_H

#ifndef __FX_STANDARDDOCUMENTPROCESSORTESTCASE_H
#define __FX_STANDARDDOCUMENTPROCESSORTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/extension/processor/StandardDocumentProcessor.h"
#include <map>
#include <vector>

FX_NS_DEF(processor);

class StandardDocumentProcessorTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(StandardDocumentProcessorTestCase);
    CPPUNIT_TEST(testProcess);
    CPPUNIT_TEST(testProcessMultiFile);
    CPPUNIT_TEST(testProcessFileMisField);
    CPPUNIT_TEST(testProcessWithEmptyField);
    CPPUNIT_TEST_SUITE_END();
public:
    StandardDocumentProcessorTestCase();
    ~StandardDocumentProcessorTestCase();

    void setUp();
    void tearDown();

protected:
    void testProcess();
    void testProcessMultiFile();
    void testProcessFileMisField();
    void testProcessWithEmptyField();

private:
    typedef std::vector<std::pair<std::string, std::string> > Answer;

    void makeAnswer(const std::string& str, Answer& ans);
    std::string writeTestFile(const std::string& sFileName,
                              const char* szFileContent);

private:
    FX_NS(document)::DocumentSchemaPtr m_pDocSchema;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_STANDARDDOCUMENTPROCESSORTESTCASE_H

#ifndef __FX_TRECDOCUMENTPROCESSORTESTCASE_H
#define __FX_TRECDOCUMENTPROCESSORTESTCASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "cppunit/extensions/HelperMacros.h"
#include "firtex/extension/processor/TrecDocumentProcessor.h"
#include "firtex/extension/processor/DocumentTemplate.h"

FX_NS_DEF(processor);

class TrecDocumentProcessorTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TrecDocumentProcessorTestCase);
    CPPUNIT_TEST(testProcess);
    CPPUNIT_TEST(testProcessMultiFile);
    CPPUNIT_TEST(testProcessFileMisField);
    CPPUNIT_TEST(testProcessGZipFile);
    CPPUNIT_TEST(testProcessTrecFileWithMeta);
    CPPUNIT_TEST_SUITE_END();
public:
    TrecDocumentProcessorTestCase();
    ~TrecDocumentProcessorTestCase();

    void setUp();
    void tearDown();

protected:
    void testProcess();
    void testProcessMultiFile();
    void testProcessFileMisField();
    void testProcessGZipFile();
    void testProcessTrecFileWithMeta();

private:
    typedef std::vector<std::pair<std::string, std::string> > Answer;

    void makeAnswer(const std::string& str, Answer& ans);
    std::string writeTestFile(const std::string& sFileName,
                              const char* szFileContent);
    std::string getTestPath();
    std::string getTestOutputPath();

private:
    FX_NS(document)::DocumentSchemaPtr m_pDocSchema;
    DocumentTemplatePtr m_pDocTemp;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_TRECDOCUMENTPROCESSORTESTCASE_H

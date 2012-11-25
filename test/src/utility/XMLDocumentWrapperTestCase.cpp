#include "utility/XMLDocumentWrapperTestCase.h"
#include "firtex/utility/XMLDocumentWrapper.h"

using namespace std;

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, XMLDocumentWrapperTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(XMLDocumentWrapperTestCase);

XMLDocumentWrapperTestCase::XMLDocumentWrapperTestCase() 
{
}

XMLDocumentWrapperTestCase::~XMLDocumentWrapperTestCase() 
{
}

void XMLDocumentWrapperTestCase::setUp()
{
}

void XMLDocumentWrapperTestCase::tearDown()
{
}

void XMLDocumentWrapperTestCase::testParse()
{
    std::string sXml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       "<root>\n"
                       "\t<global>\n"
                       "\t\t<general>\n"
                       "\t\t\t<dict id=\"id_attr_value\">dict_value</dict>\n"
                       "\t\t</general>\n"
                       "\t</global>\n"
                       "</root>\n\n";
    XMLDocumentWrapper xmlDoc;
    try
    {
        xmlDoc.parse(sXml);
    }
    catch(const BadXmlFormatException& )
    {
        CPPUNIT_ASSERT(false);
    }

    XMLNodeWrapperPtr firstNode = xmlDoc.firstNode();
    CPPUNIT_ASSERT(firstNode.isNotNull());
    CPPUNIT_ASSERT_EQUAL(std::string("root"), std::string(firstNode->getName()));

    string sPrint;
    xmlDoc.print(sPrint);
    CPPUNIT_ASSERT_EQUAL(sXml, sPrint);
}

void XMLDocumentWrapperTestCase::testPrint()
{
    XMLDocumentWrapper xmlDoc;
    xmlDoc.addDeclarationNode("UTF-8");

    xmlDoc.appendNode(XMLDocumentWrapper::NODE_ELEMENT, "root");
    XMLNodeWrapperPtr pRoot = xmlDoc.firstNode();
    pRoot->appendNode(XMLDocumentWrapper::NODE_ELEMENT, "global", "testvalue");
    pRoot->appendAttribute("id", "testid");
        
    ostringstream ss;
    xmlDoc.print(ss);

    string sExpect = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                     "<root id=\"testid\">\n"
                     "\t<global>testvalue</global>\n"
                     "</root>\n\n";
    CPPUNIT_ASSERT_EQUAL(sExpect, ss.str());
}

void XMLDocumentWrapperTestCase::testClearAndReuse()
{
    XMLDocumentWrapper xmlDoc;

    string sExpect = "<result>\n"
                     "\t<hits number_hits=\"1\">\n"
                     "\t\t<hit>hit_value1</hit>\n"
                     "\t</hits>\n"
                     "\t<hits number_hits=\"2\">\n"
                     "\t\t<hit>hit_value2</hit>\n"
                     "\t</hits>\n"
                     "</result>\n\n";
                 
    for (size_t i = 0; i < 2; ++i)
    {
        XMLNodeWrapperPtr pRootNode = xmlDoc.appendNode(
                XMLDocumentWrapper::NODE_ELEMENT, "result");

        XMLNodeWrapperPtr pHitsNode = pRootNode->appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, "hits");
        pHitsNode->appendAttribute("number_hits", "1");
        pHitsNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                              "hit", "hit_value1");
        
        pHitsNode = pRootNode->appendNode(
                XMLDocumentWrapper::NODE_ELEMENT, "hits");
        pHitsNode->appendAttribute("number_hits", "2");
        pHitsNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                              "hit", "hit_value2");

        ostringstream ss;
        xmlDoc.print(ss);
        CPPUNIT_ASSERT_EQUAL(sExpect, ss.str());

        xmlDoc.clear();
    }
}

FX_NS_END


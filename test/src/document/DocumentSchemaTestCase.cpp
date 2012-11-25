#include "document/DocumentSchemaTestCase.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/config/XMLConfigurator.h"

using namespace std;
FX_NS_USE(config);

FX_NS_DEF(document);

SETUP_STREAM_LOGGER(document, DocumentSchemaTestCase);
CPPUNIT_TEST_SUITE_REGISTRATION(DocumentSchemaTestCase);

DocumentSchemaTestCase::DocumentSchemaTestCase() 
{
}

DocumentSchemaTestCase::~DocumentSchemaTestCase() 
{
}

void DocumentSchemaTestCase::setUp()
{
}

void DocumentSchemaTestCase::tearDown()
{
}

void DocumentSchemaTestCase::testReadSchema()
{
    string sSchema = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                     "<configure>"
                     "<index_schema>"
                     "<fields>"
                     "<field>"
                     "<name>Number1</name>"
                     "<type>INT32</type>"
                     "<store>true</store>"
                     "<sortable>true</sortable>"
                     "</field>"
                     "<field>"
                     "<name>Number2</name>"
                     "<type>INT64</type>"
                     "<store>false</store>"
                     "<sortable>true</sortable>"
                     "</field>"
                     "<field>"
                     "<name>Price</name>"
                     "<type>FLOAT</type>"
                     "<store>true</store>"
                     "<sortable>true</sortable>"
                     "</field>"
                     "</fields>"
                     "</index_schema>"
                     "</configure>";

    // schema.addSortableField(_T("Number1"), FieldType::INT32, true);
    // schema.addSortableField(_T("Number2"), FieldType::INT64, true);
    // schema.addSortableField(_T("Price"), FieldType::FLOAT, true);

    XMLConfigurator configurator;
    configurator.loadFromBuffer(sSchema);

    DocumentSchema schema;
    schema.configure(configurator);

    const FieldSchema* pFieldSchema = schema.getSchema("Number1");
    CPPUNIT_ASSERT(pFieldSchema != NULL);
    CPPUNIT_ASSERT(pFieldSchema->hasForwardIndex() == true);
    CPPUNIT_ASSERT(pFieldSchema->isStored() == true);

    pFieldSchema = schema.getSchema("Number2");
    CPPUNIT_ASSERT(pFieldSchema != NULL);
    CPPUNIT_ASSERT(pFieldSchema->hasForwardIndex() == true);
    CPPUNIT_ASSERT(pFieldSchema->isStored() == false);

    pFieldSchema = schema.getSchema("Price");
    CPPUNIT_ASSERT(pFieldSchema != NULL);
    CPPUNIT_ASSERT(pFieldSchema->hasForwardIndex() == true);
    CPPUNIT_ASSERT(pFieldSchema->isStored() == true);
}

FX_NS_END


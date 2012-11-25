#include "DocumentTemplateTestCase.h"
#include <memory>
#include "../src/TestHelper.h"
#include "firtex/extension/processor/DocumentTemplate.h"
#include "firtex/common/Any.h"

FX_NS_USE(config);
FX_NS_USE(common);
using namespace std;

FX_NS_DEF(processor);

CPPUNIT_TEST_SUITE_REGISTRATION(DocumentTemplateTestCase);


DocumentTemplateTestCase::DocumentTemplateTestCase(void)
{
}

DocumentTemplateTestCase::~DocumentTemplateTestCase(void)
{
}

void DocumentTemplateTestCase::setUp()
{

}

void DocumentTemplateTestCase::tearDown()
{

}

void DocumentTemplateTestCase::testToConf()
{
    DocumentTemplate temp;
    temp.setDocTag("DOC");

    temp.addMeta("Date", "modified");
    temp.addMeta("title", "TITLE");

    temp.addTag("field1", "TITLE");
    temp.addTag("field2", "BODY", "TEXT");

    Configurator c;
    temp.configure(c);

    string strMap = toString(c.getMap());
    string expectStr = "template: \n";
    expectStr += "   doc_tag: DOC\n";
    expectStr += "   embedded_field: \n";
    expectStr += "   metadata: \n";
    expectStr += "      meta: \n";
    expectStr += "         name: Date\n";
    expectStr += "         field_name: modified\n";
    expectStr += "      meta: \n";
    expectStr += "         name: title\n";
    expectStr += "         field_name: TITLE\n";
    expectStr += "   tags: \n";
    expectStr += "      tag: \n";
    expectStr += "         name: field1\n";
    expectStr += "         field_name: TITLE\n";
    expectStr += "         alias: \n";
    expectStr += "      tag: \n";
    expectStr += "         name: field2\n";
    expectStr += "         field_name: BODY\n";
    expectStr += "         alias: TEXT\n";

    CPPUNIT_ASSERT_EQUAL(expectStr, strMap);
}

void DocumentTemplateTestCase::testFromConf()
{
    DocumentTemplate temp1;
    temp1.setDocTag("DOC");

    temp1.addMeta("Date", "modified");
    temp1.addMeta("title", "TITLE");

    temp1.addTag("field1", "TITLE");
    temp1.addTag("field2", "BODY", "TEXT");

    Configurator c;
    temp1.configure(c);

    DocumentTemplate temp2;
    c.setMode(Configurator::FROM_CONF);
    temp2.configure(c);

    CPPUNIT_ASSERT_EQUAL(temp1.getDocTag(), temp2.getDocTag());
    DocumentTemplate::Iterator it1 = temp1.iterator();
    DocumentTemplate::Iterator it2 = temp2.iterator();
    CPPUNIT_ASSERT_EQUAL(it1.size(), it2.size());

    while(it1.hasNext())
    {
        CPPUNIT_ASSERT(it2.hasNext());
        const DocumentTemplate::Tag& tag1 = it1.next();
        const DocumentTemplate::Tag& tag2 = it2.next();
        CPPUNIT_ASSERT_EQUAL(tag1.getTagName(), tag2.getTagName());
        CPPUNIT_ASSERT_EQUAL(tag1.getAlias(), tag2.getAlias());
        CPPUNIT_ASSERT_EQUAL(tag1.getFieldName(), tag2.getFieldName());
    }

    const DocumentTemplate::Metadata& metadata1 = temp1.getMetadata();
    const DocumentTemplate::Metadata& metadata2 = temp2.getMetadata();
    CPPUNIT_ASSERT_EQUAL(metadata1.size(), metadata2.size());
    for (size_t i = 0; i < metadata1.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(metadata1[i].getMetaName(), metadata2[i].getMetaName());        
        CPPUNIT_ASSERT_EQUAL(metadata1[i].getFieldName(), metadata2[i].getFieldName());        
    }
}

string DocumentTemplateTestCase::toString(const Configurator::ConfMap& confMap)
{
    Configurator c(confMap);
    Configurator::Iterator rootIt = c.findConf("template");
    if (rootIt.hasNext())
    {
        string str ("template: \n");
        Configurator::ConfMap rootMap = AnyCast<Configurator::ConfMap>(rootIt.next().second);
        for (Configurator::ConfMap::const_iterator it = rootMap.begin();
             it != rootMap.end(); ++it)
        {
            str += "   ";
            str += it->first; //doc_tag
            str += ": ";
            str += AnyCast<string>(it->second);
            str += "\n";

            ++it;
            str += "   ";
            str += it->first; //embedded_field
            str += ": ";
            str += AnyCast<string>(it->second);
            str += "\n";

            ++it;
            str += "   ";
            str += it->first;//meta
            str += ": \n";

            Configurator::ConfMap subMap = AnyCast<Configurator::ConfMap>(it->second);
            for (Configurator::ConfMap::const_iterator it2 = subMap.begin();
                 it2 != subMap.end(); ++it2)
            {
                str += "      ";
                str += it2->first; //tag
                str += ": \n";
                Configurator::ConfMap subMap2 = AnyCast<Configurator::ConfMap>(it2->second);
                for (Configurator::ConfMap::const_iterator it3 = subMap2.begin();
                     it3 != subMap2.end(); ++it3)
                {
                    str += "         ";
                    str += it3->first;
                    str += ": ";
                    str += AnyCast<string>(it3->second);
                    str += "\n";                
                }
            }        

            ++it;
            str += "   ";
            str += it->first;//tags
            str += ": \n";

            Configurator::ConfMap subMap2 = AnyCast<Configurator::ConfMap>(it->second);
            for (Configurator::ConfMap::const_iterator it2 = subMap2.begin();
                 it2 != subMap2.end(); ++it2)
            {
                str += "      ";
                str += it2->first; //tag
                str += ": \n";
                Configurator::ConfMap subMap2 = AnyCast<Configurator::ConfMap>(it2->second);
                for (Configurator::ConfMap::const_iterator it3 = subMap2.begin();
                     it3 != subMap2.end(); ++it3)
                {
                    str += "         ";
                    str += it3->first;
                    str += ": ";
                    str += AnyCast<string>(it3->second);
                    str += "\n";                
                }
            }        
        }
        return str;
    }
    return "";
}

FX_NS_END

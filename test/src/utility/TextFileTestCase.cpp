#include "TextFileTestCase.h"
//#include "firtex/utility/TextFile.h"
#include "TestHelper.h"

FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(TextFileTestCase);

TextFileTestCase::TextFileTestCase(void)
{
}

TextFileTestCase::~TextFileTestCase(void)
{
}

void TextFileTestCase::testWrite()
{
	/*FX_NS(utility)::TextFile tf;
	tf.open("./test.txt","w+t");
	CPPUNIT_ASSERT( tf.isFileOpen() );
	char szText[100];	
	for(size_t i = 0;i < 10000;i++)
	{
		sprintf(szText,"%s %d\n","test string",i);
		tf.write(szText);
		tf.write(szText,strlen(szText));
		tf.write(szText,false);
		sprintf(szText,"%s %d","test string",i);
		tf.write(szText,true);
	}	
	tf.close();*/
}

void TextFileTestCase::testRead()
{
	/*FX_NS(utility)::TextFile tf;
	tf.open("./test.txt","r+t");
	CPPUNIT_ASSERT( tf.isFileOpen() );	
	char szText[100];
	char szTmp[100];
	int ret;
	for(size_t i = 0;i < 10000;i++)
	{
		sprintf(szTmp,"%s %d\n","test string",i);
		tf.read(szText,100);
		ret = strcmp(szText,szTmp);
		CPPUNIT_ASSERT( ret == 0);
		tf.read(szText,100);
		ret = strcmp(szText,szTmp);
		CPPUNIT_ASSERT( ret == 0);
		tf.read(szText,100);
		ret = strcmp(szText,szTmp);
		CPPUNIT_ASSERT( ret == 0);
		tf.read(szText,100);
		ret = strcmp(szText,szTmp);
		CPPUNIT_ASSERT( ret == 0);
	}
	tf.close();*/
}

FX_NS_END

// Firtex测试
// tester : 关峰
// E-mail : guanfeng@software.ict.ac.cn
// 日  期 : 2009/3/26
//

/**
 * \if CN
 * 检测索引内容是否正确，即检查.dfp以及.pop的内容是否正确
 * \endif
 */

#ifndef _INDEXCONTENTTESTCASE_H
#define _INDEXCONTENTTESTCASE_H


#include "cppunit/extensions/HelperMacros.h"
#include "firtex/utility/Path.h"

FX_NS_DEF(index);

class IndexContentTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( IndexContentTestCase );
	CPPUNIT_TEST( testIndexContent_WL );
	//CPPUNIT_TEST( testIndexContent_DL );//TODO:
	//CPPUNIT_TEST( testIndexContent_DL0 );//TODO:
	CPPUNIT_TEST_SUITE_END();

public:
	IndexContentTestCase(void);
	~IndexContentTestCase(void);

public:
    void setUp();
	void tearDown();
protected:
	/**
	 * \if CN
	 * 测试词级别的索引内容。
	 * 检查每个词的倒排链中的位置信息是否正确，以及总词数是否正确
	 * 测试用的文档集合以及索引文件默认位置为：
	 * bin/testDocs/WL, bin/testIndex/WL
	 * \endif
	 */
	void testIndexContent_WL();

	/**
	 * \if CN
	 * 测试文档级别的索引内容。
	 * 检查每个词的倒排链中的文档id和词频是否正确
	 * 测试用的文档集合以及索引文件默认位置为：
	 * bin/testDocs/DL, bin/testIndex/DL
	 * \endif
	 */
	void testIndexContent_DL();

	/**
	 * \if CN
	 * 测试文档级别0的索引内容。
	 * 检查每个词的倒排链中的文档id是否正确
	 * 测试用的文档集合以及索引文件默认位置为：
	 * bin/testDocs/DL0, bin/testIndex/DL0
	 * \endif
	 */
	void testIndexContent_DL0();

private:
	DocumentScanner		*m_pDocScanner;
    FX_NS(utility)::Path m_testFilePath;    
};

FX_NS_END

#endif

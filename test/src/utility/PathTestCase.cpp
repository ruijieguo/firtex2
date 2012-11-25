#include "utility/PathTestCase.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/Path.h"
#include <stdio.h>

FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(PathTestCase);

PathTestCase::PathTestCase(void)
{	
    m_pPath = NULL;
}

PathTestCase::~PathTestCase(void)
{	
}


void PathTestCase::setUp()
{
    m_pPath = new Path();
}

void PathTestCase::tearDown()
{
    delete m_pPath;
}

void PathTestCase::testToString()
{
#ifdef FX_WINDOWS
    tstring str = _T("F:/test/test.txt");
    m_pPath->assign(str);
	str = _T("F:\\test\\test.txt");
    CPPUNIT_ASSERT_EQUAL((tstring)str, m_pPath->toString());

    str = _T("F:\\test\\test.txt");
    m_pPath->assign(str);
    CPPUNIT_ASSERT_EQUAL((tstring)str, m_pPath->toString());

    str = _T("F:\\test\\");
    m_pPath->assign(str);
    str = _T("F:\\test");
    CPPUNIT_ASSERT_EQUAL((tstring)str, m_pPath->toString());
#else
    tstring str = _T("/home/test/test.txt");
	m_pPath->assign(str);
    CPPUNIT_ASSERT_EQUAL((tstring)str, m_pPath->toString());

    str = _T("/home/test/");
	m_pPath->assign(str);
    str = _T("/home/test");
    CPPUNIT_ASSERT_EQUAL((tstring)str, m_pPath->toString());
#endif
}

void PathTestCase::testRelative()
{
    m_pPath->assign(_T("../test/test.txt"));
    CPPUNIT_ASSERT(m_pPath->isRelative());
    m_pPath->assign(_T("./test/test.txt"));
    CPPUNIT_ASSERT(m_pPath->isRelative());
    m_pPath->assign(_T(".\\test\\test.txt"));
    CPPUNIT_ASSERT(m_pPath->isRelative());
    m_pPath->assign(_T("..\\test\\test.txt"));
    CPPUNIT_ASSERT(m_pPath->isRelative());

#ifdef FX_WINDOWS
    m_pPath->assign(_T("c:/test/test.txt"));
    CPPUNIT_ASSERT(!m_pPath->isRelative());
#else
    m_pPath->assign(_T("/home/test/test.txt"));
    CPPUNIT_ASSERT(!m_pPath->isRelative());
#endif

}

FX_NS_END

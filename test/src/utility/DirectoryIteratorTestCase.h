#ifndef __DIRECTORYITERATORTESTCASE_H
#define __DIRECTORYITERATORTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/utility/File.h"
#include <vector>

FX_NS_DEF(utility);

class DirectoryIteratorTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( DirectoryIteratorTestCase );
    CPPUNIT_TEST(testNext);
    CPPUNIT_TEST(testNoRecursiveNext);
	CPPUNIT_TEST_SUITE_END();
public:
	DirectoryIteratorTestCase(void);
	~DirectoryIteratorTestCase(void);

public:
	void setUp();
	void tearDown();
    void testNext();
    void testNoRecursiveNext();    
protected:
    void addFile(const FX_NS(utility)::File& file);    
    void visitFile(const FX_NS(utility)::File& file);    
    bool isAllVisited();    
protected:
    struct FileVisitor
    {
      FX_NS(utility)::File file;
      bool visited;
    };
    std::vector<FileVisitor> m_fileList;
};


FX_NS_END

#endif

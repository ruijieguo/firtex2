#include "DirectoryIteratorTestCase.h"
#include "firtex/utility/DirectoryIterator.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include <stdio.h>
#include "TestHelper.h"

FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(DirectoryIteratorTestCase);

DirectoryIteratorTestCase::DirectoryIteratorTestCase(void)
{	
}

DirectoryIteratorTestCase::~DirectoryIteratorTestCase(void)
{	
}

void DirectoryIteratorTestCase::setUp()
{
    m_fileList.clear();  
    tstring str = TestHelper::getTestDataPath();
    Path path(str);
    path.makeDirectory();
    path.pushDirectory(_T("dir_test"));
    File f(path);
    f.createDirectory();

    path.setFileName(_T("file1.txt"));
    f = path;
    f.createFile();
    addFile(f);

    path.setFileName(_T("file2.txt"));
    f = path;
    f.createFile();
    addFile(f);

    path.pushDirectory(_T("sub_dir1"));
    path.setFileName(_T(""));
    f = path;
    f.createDirectory();
    addFile(f);

    path.setFileName(_T("file3.txt"));
    f = path;
    f.createFile();

    path.popDirectory();
    path.pushDirectory(_T("sub_dir2"));
    path.setFileName(_T(""));
    f = path;
    f.createDirectory();
    addFile(f);
}

void DirectoryIteratorTestCase::tearDown()
{
    tstring str = TestHelper::getTestDataPath();
    Path path(str);
    path.makeDirectory();
    path.pushDirectory(_T("dir_test"));
    File f(path);
    f.remove(true);
}

void DirectoryIteratorTestCase::testNext()
{
  Path path = TestHelper::getTestDataPath();
    path.makeDirectory();
    path.pushDirectory(_T("dir_test"));
    DirectoryIterator di(path, true);
    CPPUNIT_ASSERT(di.hasNext());
    visitFile(di.next());
    
    CPPUNIT_ASSERT(di.hasNext());
    visitFile(di.next());

    CPPUNIT_ASSERT(di.hasNext());
    visitFile(di.next());

    CPPUNIT_ASSERT(di.hasNext());
    visitFile(di.next());

    CPPUNIT_ASSERT(di.hasNext());
    visitFile(di.next());
    CPPUNIT_ASSERT(isAllVisited());
}

void DirectoryIteratorTestCase::testNoRecursiveNext()
{
    Path path = TestHelper::getTestDataPath();
    path.makeDirectory();
    path.pushDirectory(_T("dir_test"));

    DirectoryIterator di(path, false);
    CPPUNIT_ASSERT(di.hasNext());
    visitFile(di.next());
    
    CPPUNIT_ASSERT(di.hasNext());
    visitFile(di.next());

    CPPUNIT_ASSERT(di.hasNext());
    visitFile(di.next());

    CPPUNIT_ASSERT(di.hasNext());
    visitFile(di.next());

    CPPUNIT_ASSERT(isAllVisited());
}

void DirectoryIteratorTestCase::addFile(const File& file)
{
  FileVisitor fv;
  fv.file = file;
  fv.visited = false;  
  m_fileList.push_back(fv);  
}

void DirectoryIteratorTestCase::visitFile(const File& file)
{
  for (std::vector<FileVisitor>::iterator it = m_fileList.begin();
       it != m_fileList.end(); it++)
    {
      if ((*it).file == file)
        {
          (*it).visited = true;
          break;          
        }
    }  
}

bool DirectoryIteratorTestCase::isAllVisited() 
{
    bool bAllVisited = true;  
    for (std::vector<FileVisitor>::iterator it = m_fileList.begin();
       it != m_fileList.end(); it++)
    {
      if ((*it).visited == false)
        {
          bAllVisited = false;
          break;          
        }
    }  
    return bAllVisited;
}
 

FX_NS_END

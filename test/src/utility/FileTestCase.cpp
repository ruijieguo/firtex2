#include "utility/FileTestCase.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include <stdio.h>
#include "TestHelper.h"

FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(FileTestCase);

FileTestCase::FileTestCase(void)
{	
    m_pFile = NULL;
}

FileTestCase::~FileTestCase(void)
{	
}


void FileTestCase::setUp()
{
    m_pFile = new File();
  Path path = TestHelper::getTestDataPath();
  path.makeDirectory();
  path.pushDirectory(_T("testfile_dir"));
  File file(path);
  CPPUNIT_ASSERT(file.createDirectory());
}

void FileTestCase::tearDown()
{
    delete m_pFile;
  Path path = TestHelper::getTestDataPath();
  path.makeDirectory();
  path.setFileName(_T("testfilecreate.txt"));
  File file(path);
  if (file.exists())
  {
    file.remove();
  }
  path.setFileName(_T(""));
  path.pushDirectory(_T("testcreatedir"));
  file = path;
  if (file.exists())
  {
    file.remove();
  }

  path.popDirectory();
  path.pushDirectory(_T("testfile_dir"));
  file = path;
  if (file.exists())
    { 
      file.remove();
    }
}

void FileTestCase::testCreateFile()
{
  Path path = TestHelper::getTestDataPath();
  path.makeDirectory();
  path.setFileName(_T("testfilecreate.txt"));
  File file(path);
  CPPUNIT_ASSERT(file.createFile());
  CPPUNIT_ASSERT(file.exists());
}

void FileTestCase::testCreateDirectory()
{
  Path path = TestHelper::getTestDataPath();
  path.makeDirectory();
  path.pushDirectory(_T("testcreatedir"));
  File file(path);
  CPPUNIT_ASSERT(file.createDirectory());
  CPPUNIT_ASSERT(file.exists());
}

void FileTestCase::testRemoveEmptyDirectory()
{
  Path path = TestHelper::getTestDataPath();
  path.makeDirectory();
  path.pushDirectory(_T("testfile_dir"));
  File file(path);
  file.remove();
  CPPUNIT_ASSERT(!file.exists());
}

FX_NS_END

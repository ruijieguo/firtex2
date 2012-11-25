#include "TestHelper.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"

#ifndef FX_WINDOWS
#include "test.h"
#endif

FX_NS_USE(utility);

tstring TestHelper::getTestDataPath()
{
#ifdef FX_WINDOWS
    Path p(GLOBAL_CONF().General.path);
    p.makeDirectory();
    p.popDirectory();
    p.pushDirectory("testdata");
    p.makeAbsolute(p);
    return p.toString();
#else
    return TEST_DATA_PATH;
#endif
}

tstring TestHelper::getTestOutputDir()
{
#ifdef FX_WINDOWS
    Path p(GLOBAL_CONF().General.path);
    p.makeDirectory();
    p.popDirectory();
    p.pushDirectory("test_output");
    p.makeAbsolute(p);
    return p.toString();
#else
    return TEST_OUTPUT_PATH;
#endif
}

tstring TestHelper::getTestOutputPath(const tstring& sFileName, bool bDir)
{
    tstring str = getTestOutputDir();
    Path p(str);
    p.makeDirectory();
    if (bDir)
    {
        p.pushDirectory(sFileName);
    }
    else
    {
        p.setFileName(sFileName);
    }
    return p.toString();
}

void TestHelper::removeTestOutputFile(const tstring& sFileName, bool bDir)
{
    tstring str = getTestOutputPath(sFileName, bDir);
    File f(str);
    if (f.exists())
    {
        f.remove(true);
    }
}

tstring TestHelper::getTestDataPath(const tstring& sFileName, bool bDir)
{
    tstring str = getTestDataPath();
    Path p(str);
    p.makeDirectory();
    if (bDir)
    {
        p.pushDirectory(sFileName);
    }
    else
    {
        p.setFileName(sFileName);
    }
    return p.toString();
}

void TestHelper::removeTestFile(const tstring& sFileName, bool bDir)
{
    tstring str = getTestDataPath(sFileName, bDir);
    File f(str);
    if (f.exists())
    {
        f.remove(true);
    }
}


#ifndef __FX_TEST_H
#define __FX_TEST_H

#include "firtex/common/StdHeader.h"

class TestHelper
{
public:
    tstring getTestOutputDir();
    tstring getTestOutputPath(const tstring& sFileName, bool bDir);
    void removeTestOutputFile(const tstring& sFileName, bool bDir);

    static tstring getTestDataPath();
    static tstring getTestDataPath(const tstring& sFileName, bool bDir);
    static void removeTestFile(const tstring& sFileName, bool bDir);
};

#endif

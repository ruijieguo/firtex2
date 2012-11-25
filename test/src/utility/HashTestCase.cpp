#include "HashTestCase.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/utility/Hash.h"
#include <stdio.h>
#include "TestHelper.h"


FX_NS_DEF(utility);

CPPUNIT_TEST_SUITE_REGISTRATION(HashTestCase);

HashTestCase::HashTestCase(void)
{	
}

HashTestCase::~HashTestCase(void)
{	
}


void HashTestCase::setUp()
{	
}

void HashTestCase::tearDown()
{	
}

void HashTestCase::testHashString64()
{
    CPPUNIT_ASSERT(Hash::hashString64("abcdefg") != Hash::hashString64("abcdefgg"));
}


FX_NS_END

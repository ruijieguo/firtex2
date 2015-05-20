//
// Copyright(C) 2005--2009 FirteX Develop Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : ¹ùÈð½Ü(GuoRuijie)
// Email        : ruijieguo@gmail.com
// Created      : 2009/3/19
//
#ifndef __STANDARDANALYZERTESTCASE_H
#define __STANDARDANALYZERTESTCASE_H

#include <map>
#include "firtex/common/StdHeader.h"
#include "cppunit/extensions/HelperMacros.h"

FX_NS_DEF(analyzer);

class StandardAnalyzerTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( StandardAnalyzerTestCase );
//    CPPUNIT_TEST(testTokenize);
//    CPPUNIT_TEST(testTokenizeWithUnigram);
    CPPUNIT_TEST(testTokenizeWithUnigram2);
//    CPPUNIT_TEST(testTokenizeWithTwoTokens);
//    CPPUNIT_TEST(testTokenizeStrict);
//    CPPUNIT_TEST(testUnigramAlgorithm);
//    CPPUNIT_TEST(testTokenizeGBKFile);
//    CPPUNIT_TEST(testTokenizeUTF8File);
    CPPUNIT_TEST_SUITE_END();

public:
    StandardAnalyzerTestCase(void);
    ~StandardAnalyzerTestCase(void);

public:
    void testTokenize();
    void testTokenizeWithUnigram();
    void testTokenizeWithUnigram2();
    void testTokenizeWithTwoTokens();
    void testTokenizeStrict();
    void testUnigramAlgorithm();
    void testTokenizeGBKFile();
    void testTokenizeUTF8File();
private:
};

FX_NS_END

#endif

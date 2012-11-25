//
// Copyright(C) 2005--2009 FirteX Develop Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@gmail.com
// Created	: 2009/3/17
//
#ifndef __SYNONYMMAPESTCASE_H
#define __SYNONYMMAPESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/common/StdHeader.h"

FX_NS_DEF(analyzer);

class SynonymMapTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( SynonymMapTestCase );
	CPPUNIT_TEST( testGetSynonyms );
	CPPUNIT_TEST( testLoad );
	CPPUNIT_TEST( testSave );
    CPPUNIT_TEST( testAddSynonym );
	CPPUNIT_TEST_SUITE_END();
public:
	SynonymMapTestCase(void);
	~SynonymMapTestCase(void);
public:
	void testGetSynonyms();
    void testLoad();
    void testSave();
    void testAddSynonym();
};

FX_NS_END

#endif

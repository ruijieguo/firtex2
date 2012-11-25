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
// Created	: 2009/3/19
//
#ifndef __SYNONYMFILTERTESTCASE_H
#define __SYNONYMFILTERTESTCASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "firtex/common/StdHeader.h"

FX_NS_DEF(analyzer);

class SynonymFilterTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( SynonymFilterTestCase );
	CPPUNIT_TEST( testFilter );
	CPPUNIT_TEST_SUITE_END();
public:
	SynonymFilterTestCase(void);
	~SynonymFilterTestCase(void);
public:
	void testFilter();
};

FX_NS_END

#endif

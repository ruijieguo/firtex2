//
// Copyright(C) 2005--2009 FirteX Develop Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: 郭瑞杰(GuoRuijie)
// Email	: ruijieguo@gmail.com
// Created	: 2009/3/17
//
#include "SynonymFilterTestCase.h"
#include "firtex/analyzer/SynonymFilter.h"
#include <memory>

FX_NS_DEF(analyzer);

CPPUNIT_TEST_SUITE_REGISTRATION(SynonymFilterTestCase);

SynonymFilterTestCase::SynonymFilterTestCase(void)
{
}

SynonymFilterTestCase::~SynonymFilterTestCase(void)
{
}

void SynonymFilterTestCase::testFilter()
{
    SynonymFilter sf;
    SynonymMap& sm = sf.getSynonymMap();
    sm.addSynonyms("一群獾", "CETE");
    sm.addSynonyms("一瞥", "CAST,CATCH SIGHT OF,COLOR,COMPANY,DEKKO,"
                   "FLING,FORM,GLANCE,GLAUM,GLIMPSE,KIND,LOOK,MOLD,"
                   "NOTICE,PITCH,SEE,SHADE,SHAPE,SKIM,SORT,THROW,"
                   "TINT,TOSS,TROUPE,TYPE,VARIETY,一看,投掷,演员表,眼光,脱落物,铸件");
    TokenViewPtr pTokens(new TokenView);
    pTokens->addToken(_T("北京"));
    pTokens->addToken(_T("一群獾"));
    pTokens->addToken(_T("一瞥"));
    pTokens->addToken(_T("中国"));
    pTokens->addToken(_T("cete"));
    TokenSourcePtr tokenSource(new FX_NS(analyzer)::TokenSource);
    tokenSource->setOriginalView(pTokens);

    sf.filter(tokenSource);
    TokenViewPtr pSynTokens = tokenSource->getLastView();
    CPPUNIT_ASSERT(pSynTokens.isNull() != true);
    TokenView::Iterator iter = pSynTokens->iterator();	
    CPPUNIT_ASSERT(iter.hasNext());
    Token token = iter.next();
    CPPUNIT_ASSERT_EQUAL(tstring("北京"), tstring(token.getTextValue()));
    CPPUNIT_ASSERT_EQUAL((int32_t)1, token.getPosIncrement());

    CPPUNIT_ASSERT(iter.hasNext());
    token = iter.next();
    CPPUNIT_ASSERT_EQUAL(tstring("一群獾"), tstring(token.getTextValue()));
    CPPUNIT_ASSERT_EQUAL((int32_t)1, token.getPosIncrement());

    CPPUNIT_ASSERT(iter.hasNext());
    token = iter.next();
    CPPUNIT_ASSERT_EQUAL(tstring("cete"), tstring(token.getTextValue()));
    CPPUNIT_ASSERT_EQUAL((int32_t)0, token.getPosIncrement());

    CPPUNIT_ASSERT(iter.hasNext());
    token = iter.next();
    CPPUNIT_ASSERT_EQUAL(tstring("一瞥"), tstring(token.getTextValue()));
    CPPUNIT_ASSERT_EQUAL((int32_t)1, token.getPosIncrement());

    CPPUNIT_ASSERT(iter.hasNext());
    token = iter.next();
    CPPUNIT_ASSERT_EQUAL(tstring("cast"), tstring(token.getTextValue()));
    CPPUNIT_ASSERT_EQUAL((int32_t)0, token.getPosIncrement());

    for(size_t i = 0; i < 30; i++)
    {
        CPPUNIT_ASSERT(iter.hasNext());
        iter.next();
    }

    CPPUNIT_ASSERT(iter.hasNext());
    token = iter.next();
    CPPUNIT_ASSERT_EQUAL(tstring("铸件"), tstring(token.getTextValue()));
    CPPUNIT_ASSERT_EQUAL((int32_t)0, token.getPosIncrement());

    CPPUNIT_ASSERT(iter.hasNext());
    token = iter.next();
    CPPUNIT_ASSERT_EQUAL(tstring("中国"), tstring(token.getTextValue()));
    CPPUNIT_ASSERT_EQUAL((int32_t)1, token.getPosIncrement());

    CPPUNIT_ASSERT(iter.hasNext());
    token = iter.next();
    CPPUNIT_ASSERT_EQUAL(tstring("cete"), tstring(token.getTextValue()));
    CPPUNIT_ASSERT_EQUAL((int32_t)1, token.getPosIncrement());

    CPPUNIT_ASSERT(iter.hasNext());
    token = iter.next();
    CPPUNIT_ASSERT_EQUAL(tstring("一群獾"), tstring(token.getTextValue()));
    CPPUNIT_ASSERT_EQUAL((int32_t)0, token.getPosIncrement());
}

FX_NS_END

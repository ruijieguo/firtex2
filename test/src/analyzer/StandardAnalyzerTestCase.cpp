//
// Copyright(C) 2005--2009 FirteX Develop Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : Ruijie Guo
// Email        : ruijieguo@gmail.com
// Created      : 2009/3/17
//

#include "StandardAnalyzerTestCase.h"
#include "firtex/analyzer/StandardAnalyzer.h"
#include "firtex/analyzer/TokenSource.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/utility/BinaryFile.h"
#include "TestHelper.h"
#include <memory>

using namespace firtex;

FX_NS_USE(utility);

FX_NS_DEF(analyzer);

CPPUNIT_TEST_SUITE_REGISTRATION(StandardAnalyzerTestCase);

StandardAnalyzerTestCase::StandardAnalyzerTestCase(void)
{
}

StandardAnalyzerTestCase::~StandardAnalyzerTestCase(void)
{
}

void StandardAnalyzerTestCase::testTokenize()
{
    string str = "中国北京上海广州。firtex Open source激情ruijieguo@gmail.com";
    StandardAnalyzer sa;
    sa.setParam("encode=utf-8;algorithm=max_forward");
    sa.init();

    TokenViewPtr pTokenView = sa.tokenize(str.c_str());
    CPPUNIT_ASSERT(pTokenView);
    
    const static size_t NUM_TOKENS = 9;
    string answer[NUM_TOKENS] = {"中国", "北京", "上海", "广州", "firtex", "Open", 
                                 "source", "激情", "ruijieguo@gmail.com"};
    int32_t posInc[NUM_TOKENS] = {1, 1, 1, 1, 2, 1, 1, 1, 1};
    int32_t startOff[NUM_TOKENS] = {0, 6, 12, 18, 27, 34, 39, 45, 51};
    int32_t endOff[NUM_TOKENS] = {6, 12, 18, 24, 33, 38, 45, 51, 70};
    TokenView::Iterator it = pTokenView->iterator();
    CPPUNIT_ASSERT_EQUAL(NUM_TOKENS, it.size());

    size_t i = 0;
    while (it.hasNext())
    {
        const Token& token = it.next();
        CPPUNIT_ASSERT_EQUAL(answer[i], string(token.getTextValue()));
        CPPUNIT_ASSERT_EQUAL(posInc[i], token.getPosIncrement());
        CPPUNIT_ASSERT_EQUAL(startOff[i], token.getStartOffset());
        CPPUNIT_ASSERT_EQUAL(endOff[i], token.getEndOffset());

        // cout << token.getTextValue() << std::endl;
        // cout << token.getPosIncrement() << ", " << 
        //     token.getStartOffset() << ", " << token.getEndOffset() << std::endl;
        ++i;
    }
}

void StandardAnalyzerTestCase::testTokenizeWithUnigram()
{
    string str = "中国北京上海广州。\"firtex Open source激情ruijieguo@gmail.com";
    StandardAnalyzer sa;
    sa.setParam("encode=utf-8;algorithm=unigram");
    sa.init();

    TokenViewPtr pTokenView = sa.tokenize(str.c_str());
    CPPUNIT_ASSERT(pTokenView);
    
    const static size_t NUM_TOKENS = 9;
    string answer[NUM_TOKENS] = {"中国", "北京", "上海", "广州", "firtex", "Open", 
                                 "source", "激情", "ruijieguo@gmail.com"};
    int32_t posInc[NUM_TOKENS] = {1, 1, 1, 1, 3, 1, 1, 1, 1};
    int32_t startOff[NUM_TOKENS] = {0, 6, 12, 18, 28, 35, 40, 46, 52};
    int32_t endOff[NUM_TOKENS] = {6, 12, 18, 24, 34, 39, 46, 52, 71};
    TokenView::Iterator it = pTokenView->iterator();
    //CPPUNIT_ASSERT_EQUAL(NUM_TOKENS, it.size());

    size_t i = 0;
    while (it.hasNext())
    {
        const Token& token = it.next();
        CPPUNIT_ASSERT_EQUAL(answer[i], string(token.getTextValue()));
        CPPUNIT_ASSERT_EQUAL(posInc[i], token.getPosIncrement());
        CPPUNIT_ASSERT_EQUAL(startOff[i], token.getStartOffset());
        CPPUNIT_ASSERT_EQUAL(endOff[i], token.getEndOffset());

        // cout << token.getTextValue() << std::endl;
        // cout << token.getPosIncrement() << ", " << 
        //     token.getStartOffset() << ", " << token.getEndOffset() << std::endl;
        ++i;
    }
}

void StandardAnalyzerTestCase::testTokenizeWithUnigram2()
{
    string str = "月经期和服药期可以吃象西瓜，苹果等水果吗";
    StandardAnalyzer sa;
    sa.setParam("encode=utf-8;algorithm=unigram");
    sa.init();

    TokenViewPtr pTokenView = sa.tokenize(str.c_str());
    CPPUNIT_ASSERT(pTokenView);
    
    const static size_t NUM_TOKENS = 9;
    string answer[NUM_TOKENS] = {"中国", "北京", "上海", "广州", "firtex", "Open", 
                                 "source", "激情", "ruijieguo@gmail.com"};
    int32_t posInc[NUM_TOKENS] = {1, 1, 1, 1, 3, 1, 1, 1, 1};
    int32_t startOff[NUM_TOKENS] = {0, 6, 12, 18, 28, 35, 40, 46, 52};
    int32_t endOff[NUM_TOKENS] = {6, 12, 18, 24, 34, 39, 46, 52, 71};
    TokenView::Iterator it = pTokenView->iterator();
    //CPPUNIT_ASSERT_EQUAL(NUM_TOKENS, it.size());

    size_t i = 0;
    while (it.hasNext())
    {
        const Token& token = it.next();
         cout << token.getTextValue() << std::endl;
         cout << token.getPosIncrement() << ", " << 
             token.getStartOffset() << ", " << token.getEndOffset() << std::endl;
        ++i;
    }
}

void StandardAnalyzerTestCase::testTokenizeWithTwoTokens()
{
    StandardAnalyzer sa;
    sa.setParam("encode=utf-8;algorithm=max_forward");
    sa.init();
    TokenViewPtr pUnAnalyzedTokens =
        new TokenView(TokenView::MAX_TOKENS, Token::TT_CWORD);
    string str = "江西";
    pUnAnalyzedTokens->addToken(str.c_str(), str.length());
    str = "上海";
    pUnAnalyzedTokens->addToken(str.c_str(), str.length());

    TokenSourcePtr tokenSource(new FX_NS(analyzer)::TokenSource);
    tokenSource->setOriginalView(pUnAnalyzedTokens);

    TokenViewPtr pTokens = sa.tokenize(tokenSource);
    CPPUNIT_ASSERT(pTokens);

    TokenView::Iterator it = pTokens->iterator();
    CPPUNIT_ASSERT_EQUAL((size_t)2, it.size());
}

void StandardAnalyzerTestCase::testTokenizeStrict()
{
    const static size_t testCaseCount = 5;
    string testStrs[testCaseCount] = {
        "我一个人吃饭",
        "微软计划于2005年在全球范围内扩大Ｗｉｎｄｏｗｓ正版增值计划实施范围。",
        "中国航天官员应邀到美国与太空总署官员开会。",
        "Google黑板报数学之美系列二说的是中文分词。",
        "测试firtex如何对数字12345678进行分词。"
    };

    size_t wordCount[testCaseCount] = {4, 16, 10, 11, 8};
    string keys[testCaseCount][50] = {
        {"我", "一个", "人", "吃饭"},
        {"微软", "计划", "于", "2005", "年", "在", "全球", "范围",
         "内", "扩大", "Ｗｉｎｄｏｗｓ", "正版", "增值", "计划", "实施", "范围"},
        {"中国航天", "官员", "应邀", "到", "美国","与", "太空", 
         "总署", "官员", "开会"},
        {"Google", "黑板报", "数学", "之美", "系列", "二", 
         "说", "的", "是", "中文", "分词"},
        {"测试", "firtex", "如何", "对数", "字", 
         "12345678", "进行", "分词"}
    };

    StandardAnalyzer stdAnalyzer;
    stdAnalyzer.setParam("encode=utf-8;algorithm=max_forward");
    stdAnalyzer.init();

    size_t count = 0;
    for (size_t i = 0; i < testCaseCount; i++)
    {
        count = 0;

        TokenViewPtr pTokens = stdAnalyzer.tokenize(testStrs[i]);
        CPPUNIT_ASSERT(pTokens);

        TokenView::Iterator it = pTokens->iterator();

        while (it.hasNext())
        {
            const Token& token = it.next();
            CPPUNIT_ASSERT_EQUAL(keys[i][count++], string(token.getTextValue()));
  //            cout << token.getTextValue() << endl;
        }
        CPPUNIT_ASSERT(count == wordCount[i]);
    }
}

void StandardAnalyzerTestCase::testUnigramAlgorithm()
{
    string str = "对数学";

    //test unigram
    {
        StandardAnalyzer sa;
        sa.setParam("encode=utf-8;algorithm=unigram");
        sa.init();

        TokenViewPtr pTokenView = sa.tokenize(str.c_str());
        CPPUNIT_ASSERT(pTokenView);
    
        const static size_t NUM_TOKENS = 2;
        string answer[NUM_TOKENS] = {"对", "数学"};

        TokenView::Iterator it = pTokenView->iterator();
        CPPUNIT_ASSERT_EQUAL(NUM_TOKENS, it.size());

        size_t i = 0;
        while (it.hasNext())
        {
            CPPUNIT_ASSERT_EQUAL(answer[i++], string(it.next().getTextValue()));
        }
    }

    //test max forward
    {
        StandardAnalyzer sa;
        sa.setParam("encode=utf-8;algorithm=max_forward");
        sa.init();

        TokenViewPtr pTokenView = sa.tokenize(str.c_str());
        CPPUNIT_ASSERT(pTokenView);
    
        const static size_t NUM_TOKENS = 2;
        string answer[NUM_TOKENS] = {"对数", "学"};

        TokenView::Iterator it = pTokenView->iterator();
        CPPUNIT_ASSERT_EQUAL(NUM_TOKENS, it.size());

        size_t i = 0;
        while (it.hasNext())
        {
            CPPUNIT_ASSERT_EQUAL(answer[i++], string(it.next().getTextValue()));
        }
    }
}

void StandardAnalyzerTestCase::testTokenizeGBKFile()
{
    string sPath = TestHelper::getTestDataPath("txt_files", true);
    sPath += "/0.txt";
    BinaryFile bf;
    bf.open(sPath, BinaryFile::READ);
    CPPUNIT_ASSERT(bf.isFileOpen());
    
    string data;
    data.resize((size_t)bf.getLength());
    bf.read((void*)data.c_str(), data.size());
    
    StandardAnalyzer sa;
    sa.setParam("encode=gbk;algorithm=max_forward");
//    sa.setParam("encode=gbk;algorithm=unigram");
    sa.init();
    
    TokenViewPtr pTokenView = sa.tokenize(data.c_str());
    CPPUNIT_ASSERT(pTokenView);

    // TokenView::Iterator it = pTokenView->iterator();
    // while (it.hasNext())
    // {
    //     const Token& token = it.next();
    //     cout << token.getTextValue() << endl;
    //     cout << token.getPosIncrement() << ", " << 
    //         token.getStartOffset() << ", " << token.getEndOffset() << std::endl;

    // }
}

void StandardAnalyzerTestCase::testTokenizeUTF8File()
{
    string sPath = TestHelper::getTestDataPath("utf8_txt_files", true);
    sPath += "/0.txt";
    BinaryFile bf;
    bf.open(sPath, BinaryFile::READ);
    CPPUNIT_ASSERT(bf.isFileOpen());
    
    string data;
    data.resize((size_t)bf.getLength());
    bf.read((void*)data.c_str(), data.size());
    
    StandardAnalyzer sa;
    sa.setParam("encode=utf-8;algorithm=max_forward");
//    sa.setParam("encode=utf-8;algorithm=unigram");
    sa.init();
    
    TokenViewPtr pTokenView = sa.tokenize(data.c_str());
    CPPUNIT_ASSERT(pTokenView);

    // TokenView::Iterator it = pTokenView->iterator();
    // while (it.hasNext())
    // {
    //     const Token& token = it.next();
    //     cout << token.getTextValue() << endl;
    //     cout << token.getPosIncrement() << ", " << 
    //         token.getStartOffset() << ", " << token.getEndOffset() << std::endl;

    // }
}

FX_NS_END

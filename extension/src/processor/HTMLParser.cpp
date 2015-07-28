//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2007/5/1
//

#include "firtex/extension/processor/HTMLParser.h"
#include "firtex/utility/BinaryFile.h"

FX_NS_USE(utility);

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(processor, HTMLParser);

HTMLParser::HTMLParser()
    : m_pContentBuffer(NULL)
    , m_nContentBufferSize(0)
    , m_nContentLength(0)
    , m_pTitleBuffer(NULL)
    , m_nTitleBufferSize(0)
    , m_nTitleLength(0)
    , m_pReadBuffer(NULL)
    , m_nReadBufferSize(0)
    , m_nFileSize(0)
    , m_bParseHead(false)
    , m_bExtractTitle(false)
{
}

HTMLParser::~HTMLParser()
{
    if(m_pContentBuffer != NULL)
    {
        delete[] m_pContentBuffer;
        m_pContentBuffer = NULL;
    }

    if(m_pTitleBuffer != NULL)
    {
        delete[] m_pTitleBuffer;
        m_pTitleBuffer = NULL;
    }

    if(m_pReadBuffer)
    {
        delete m_pReadBuffer;
        m_pReadBuffer = NULL;
    }
}

void HTMLParser::parse(const char *szHTML, size_t nLen)
{
    if(szHTML == NULL || nLen <= 0)
        return ;

    m_nContentLength = 0;
    m_nTitleLength = 0;

    if(m_pContentBuffer == NULL)
    {
        m_nContentBufferSize = nLen + 1;
        if (m_nContentBufferSize < CONTENTGROW_SIZE)
            m_nContentBufferSize = CONTENTGROW_SIZE;
        m_pContentBuffer = new char[m_nContentBufferSize];
    }
    else if(m_nContentBufferSize < nLen + 1)
    {
        m_nContentBufferSize += CONTENTGROW_SIZE;
        if(m_nContentBufferSize < nLen + 1)
            m_nContentBufferSize = nLen + 1;
        delete[] m_pContentBuffer;
        m_pContentBuffer = new char[m_nContentBufferSize];
    }

    char* pCur = (char*)szHTML;
    char* pEnd = (char*)szHTML + nLen;
    parseBody(pCur, pEnd);
    m_pContentBuffer[m_nContentLength] = 0;
}

void HTMLParser::parse(const tstring& sHtmlFile)
{
    BinaryFile bf;
    try
    {
        bf.open(sHtmlFile, BinaryFile::READ);
    
        m_nFileSize = (size_t)bf.getLength();
        if(m_nFileSize > MAX_FILESIZE - 1)
        {
            m_nFileSize = MAX_FILESIZE - 1;
        }

        if(!m_pReadBuffer)
        {
            m_nReadBufferSize = DEFAULT_READBUFFER_SIZE;
            if(m_nReadBufferSize < m_nFileSize + 1)
                m_nReadBufferSize = m_nFileSize + 1;
            m_pReadBuffer = new char[m_nReadBufferSize];
        }
        else if(m_nFileSize + 1 > m_nReadBufferSize)
        {
            m_nReadBufferSize = m_nFileSize + 1;
            delete[] m_pReadBuffer;
            m_pReadBuffer = new char[m_nReadBufferSize];
        }
        size_t nRet = bf.read(m_pReadBuffer, m_nFileSize);
        if(nRet != m_nFileSize)
        {
            FX_LOG(WARN, "Read file [%s] error", sHtmlFile.c_str());
            bf.close();
            return;
        }
        bf.close();
        parse(m_pReadBuffer, m_nFileSize);
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Parse file: [%s] FAILED. Error message: [%s]",
               sHtmlFile.c_str(), e.what().c_str());
    }
}

const char* HTMLParser::getFullText(size_t& nLength)
{
    nLength = m_nContentLength;
    return m_pContentBuffer;
}

const char* HTMLParser::getTitle(size_t& nLength)
{
    nLength = m_nTitleLength;
    return m_pTitleBuffer;
}

void HTMLParser::setExtractTitle(bool bExtract)
{
    m_bParseHead = true;
    m_bExtractTitle = true;
}

FX_NS_END

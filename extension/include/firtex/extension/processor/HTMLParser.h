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

#ifndef __FX_HTMLPARSER_H
#define __FX_HTMLPARSER_H

#include "firtex/common/Logger.h"
#include "firtex/utility/StringUtils.h"

FX_NS_DEF(processor);

class HTMLParser
{
protected:
    enum htmlelem_t
    {
        ET_ERROR = -1,
        ET_EOF = 0,

        ET_TAG_START,   /// <tag ...
        ET_TAG_END,     /// </tag> or <tag ... />
        ET_ATTR,        /// <tag attr="value" >      

        ET_WORD,
        ET_SPACE,

        ET_DATA,         /// content of followings:

        ET_COMMENT_START,/// <!--
        ET_COMMENT_END,  ///  -->

        ET_CDATA_START,  /// "<![CDATA["
        ET_CDATA_END,    /// "]]>"

        ET_PI_START,     /// "<?"
        ET_PI_END,       /// "?>"

        ET_ENTITY_START, /// "<!ENTITY"
        ET_ENTITY_END,   /// ">"

        ET_HEAD_START,   /// "<head>"
        ET_HEAD_END,     /// "</head>"

        ET_STYLE_START,  /// "<style"
        ET_STYLE_END,    /// ">"

        ET_SCRIPT_START, /// "<script"
        ET_SCRIPT_END,   /// ">"

        ET_QUOTATION_START, /// "
        ET_QUOTATION_END,   /// "

        ET_DQUOTATION_START,/// '
        ET_DQUOTATION_END   /// '
    };

public:
    HTMLParser();
    virtual ~HTMLParser();

public:
    /** 
     * parse HTML page 
     * @param sHtmlBuf HTML page buffer
     * @param nLen length of buffer
     */
    void parse(const char* szHTML, size_t nLen);

    /** 
     * parse HTML file
     * @param sHtmlFile HTML file path
     */
    void parse(const std::string& sHtmlFile);

    /**
     * get the text content of HTML page
     * @param nLength length of text
     * @return address of text	 
     */
    const char* getFullText(size_t& nLength);

    /**
     * get the title text of HTML page
     * @param nLength length of title
     * @return address of title
     */
    const char* getTitle(size_t& nLength);

    /**
     * extract title field
     * @param bExtract extract title field if true
     */
    void setExtractTitle(bool bExtract);

    /**
     * does title field need to be extracted or not?
     * @return true if extract title field
     */
    bool isExtractTitle(){return m_bExtractTitle;}

protected:
    inline void parseBody(char*& pCur,char* pEnd);
    inline void parseTag(char*& pCur,char* pEnd);
    inline void parseHead(char*& pCur,char* pEnd);
    inline void parseDQuotation(char*& pCur,char* pEnd);
    inline void parseQuotation(char*& pCur,char* pEnd);
    inline void parseScript(char*& pCur,char* pEnd);
    inline void parseStyle(char*& pCur,char* pEnd);
    inline char parseEntity(char*& pCur,char* pEnd);
    inline void parseEntityDecl(char*& pCur,char* pEnd);
    inline void parseCData(char*& pCur,char* pEnd);
    inline void parsePI(char*& pCur,char* pEnd);
    inline void parseComment(char*& pCur,char* pEnd);

private:
    inline void appendText(char ch);
    inline void appendTitle(char ch);
    inline bool _strncmp(const char* s, const char* s1, size_t length);

    inline bool isWhiteSpace(char c) ;
    inline bool isLetter(char ch);
    inline bool isLowerLetter(char ch);
    inline bool isDigit(char ch);

    inline void toLower(char *s, int nLen);

private:
    const static size_t MAX_FILESIZE = 1024 * 1024; //1M
    const static size_t DEFAULT_READBUFFER_SIZE = 100*1024;//100k
    const static size_t CONTENTGROW_SIZE = 32678;//32k
    const static size_t TITLEGROW_SIZE = 1024;//32k

private:
    char* m_pContentBuffer;
    size_t m_nContentBufferSize;
    size_t m_nContentLength;

    char* m_pTitleBuffer;
    size_t m_nTitleBufferSize;
    size_t m_nTitleLength;

    char* m_pReadBuffer;
    size_t m_nReadBufferSize;
    size_t m_nFileSize;

    bool m_bParseHead;
    bool m_bExtractTitle;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(HTMLParser);

//////////////////////////////////////////////////////////////////////////
//inline function
inline void HTMLParser::toLower(char *s, int nLen)
{
    for(int i=0; i<nLen; i++)
    {
        if(isLetter(s[i]))
            s[i] = tolower(s[i]);
    }
}

inline bool HTMLParser::_strncmp(const char* s, const char* s1, size_t length)
{
    switch(length)
    {
    case 8: if(s1[7] != s[7]) return false;
    case 7: if(s1[6] != s[6]) return false;
    case 6: if(s1[5] != s[5]) return false;
    case 5: if(s1[4] != s[4]) return false;
    case 4: if(s1[3] != s[3]) return false;
    case 3: if(s1[2] != s[2]) return false;
    case 2: if(s1[1] != s[1]) return false;
    case 1: if(s1[0] != s[0]) return false;
    case 0: return true;
    default: return FX_NS(utility)::strCompare(s,s1,length) == 0;
    }
}


inline void HTMLParser::appendText(char ch)
{
    if(m_nContentLength >= m_nContentBufferSize)
    {
        m_nContentBufferSize += CONTENTGROW_SIZE;
        char* pNewBuffer = new char[m_nContentBufferSize];
        memcpy(pNewBuffer,m_pContentBuffer,m_nContentLength);
        m_pContentBuffer = pNewBuffer;
    }
    m_pContentBuffer[m_nContentLength++] = ch;
}

inline void HTMLParser::appendTitle(char ch)
{
    if(m_nTitleLength >= m_nTitleBufferSize)
    {
        m_nTitleBufferSize += TITLEGROW_SIZE;
        if (m_nTitleLength > m_nTitleBufferSize)
        {
            m_nTitleBufferSize = m_nTitleLength;
        }

        char* pNewBuffer = new char[m_nTitleBufferSize];
        if (m_pTitleBuffer)
        {
            memcpy(pNewBuffer, m_pTitleBuffer, m_nTitleLength);
            delete[] m_pTitleBuffer;
        }
        m_pTitleBuffer = pNewBuffer;
    }
    m_pTitleBuffer[m_nTitleLength++] = ch;
}

inline bool HTMLParser::isWhiteSpace(char c) 
{
    return (((c) <= ' ') && ((c) == ' ' || (c) == '\t' 
                             || (c) == '\n' || (c) == '\r' || (c) == '\f'));
}
inline bool HTMLParser::isLetter(char ch)
{
    return (ch<='Z' && ch>='A') || (ch<='z' && ch>='a');
}

inline bool HTMLParser::isLowerLetter(char ch)
{
    return (ch<='z' && ch>='a');
}

inline bool HTMLParser::isDigit(char ch)
{
    return ch<='9' && ch>='0';
}

inline void HTMLParser::parseBody(char*& pCur,char* pEnd)
{
    static bool bWS = false;
    bool bWS2 = false;
    while (pCur < pEnd)
    {
        if(*pCur == '<')
        {
            parseTag(pCur,pEnd);
            continue;
        }
        else if(*pCur == '&')
        {
            appendText(parseEntity(pCur,pEnd));
            continue;
        }
        else
            bWS2 = isWhiteSpace(*pCur);
        if(!bWS2 || !bWS)
            appendText(*pCur);
        bWS = bWS2;
        pCur++;				
    }			
}
inline void HTMLParser::parseHead(char*& pCur,char* pEnd)
{
    pCur += 5; ///skip "head>"
    bool bExtract = m_bExtractTitle;
    while( pCur < pEnd - 1 )
    {
        if( bExtract && !FX_NS(utility)::strCompareNoCase(pCur,"<title>",7) )
        {
            pCur += 7;///skip "<title>"
            while(pCur < pEnd)
            {
                if(*pCur != '<')
                    appendTitle(*pCur++);
                else if( !FX_NS(utility)::strCompareNoCase(pCur,"</title>",8) )
                {
                    pCur += 8;
                    bExtract = false;
                    break;
                }
                else
                    appendTitle(*pCur++);
            }
        }
        if(*pCur == '<' && *(pCur+1) == '/')
        {
            if( !FX_NS(utility)::strCompareNoCase(pCur,"</head>",7) )
            {
                pCur += 7;
                break;
            }
        }
        pCur++;
    }
}

#define ADD_TAG()                               \
    if(!pTag)                                   \
    {pTag = pCur;pTagEnd = pTag + 1;}           \
    else pTagEnd++;


inline void HTMLParser::parseTag(char*& pCur,char* pEnd)
{
    pCur++;
    if(pCur < pEnd)
    {
        if((*pCur != '?') && (*pCur != '/') && (*pCur != '!') 
           && (isWhiteSpace(*pCur) || !isLetter(*pCur)))
        {
            appendText(*(pCur-1)); ///something like "< tag..." or "<#tag"
            //parseBody(pCur,pEnd);
            return;
        }
        htmlelem_t eCurToken = ET_TAG_START;
        if(*pCur == '/')
            pCur++;

        bool bParseHead = m_bParseHead;
        char* pTag = NULL;
        char* pTagEnd = NULL;
        while ((pCur < pEnd) && (eCurToken != ET_TAG_END))
        {
            if(isWhiteSpace(*pCur)) ///tag name ends here
            {
                break;
            }

            if(*pCur == '>')
            {
                eCurToken = ET_TAG_END;
                break;
            }
            if(*pCur == '?')
            {
                parsePI(pCur,pEnd);
                eCurToken = ET_TAG_END;
                break;
            }

            ADD_TAG();
            switch(pTagEnd - pTag)
            {
            case 8:
                if( _strncmp(pTag,"![CDATA[",8) ) 
                {
                    pCur = pTag;
                    parseCData(pCur,pEnd);
                    eCurToken = ET_TAG_END;
                }
                break;
            case 7:
                if(_strncmp(pTag,"!ENTITY",7) ) 
                {
                    pCur = pTag;
                    parseEntityDecl(pCur,pEnd);
                    eCurToken = ET_TAG_END;
                }
                break;
            case 6:
                if(!FX_NS(utility)::strCompareNoCase(pTag,"script ",7))
                {
                    pCur = pTag;
                    parseScript(pCur,pEnd);
                    eCurToken = ET_TAG_END;
                }
                break;
            case 5:
                if(!FX_NS(utility)::strCompareNoCase(pTag,"style ",6))
                {
                    pCur = pTag;
                    parseStyle(pCur,pEnd);
                    eCurToken = ET_TAG_END;
                }
                break;

            case 4:
                if(bParseHead && !FX_NS(utility)::strCompareNoCase(pTag, "head", 4))
                {
                    pCur = pTag;
                    parseHead(pCur, pEnd);
                    --pCur;
                    eCurToken = ET_TAG_END;
                }
                break;

            case 3:
                if(_strncmp(pTag,"!--",3))
                {
                    pCur = pTag;
                    parseComment(pCur, pEnd);
                    eCurToken = ET_TAG_END;
                }
                break;
            }

            pCur++;
        }

        if(eCurToken != ET_TAG_END)
        {
            while (pCur < pEnd)
            {
                if(*pCur == '\"')
                {
                    parseDQuotation(pCur,pEnd);
                }
                else if(*pCur == '\'')
                {
                    parseQuotation(pCur,pEnd);
                }
                if(*pCur == '>')
                {
                    eCurToken = ET_TAG_END;
                    break;
                }
                pCur++;
            }
        }

        if(pCur < pEnd)
        {
            if(*pCur == '>')
            {
                pCur++;
            }
        }
    }
}
inline void HTMLParser::parseDQuotation(char*& pCur,char* pEnd)
{
    pCur++;
    while (pCur < pEnd)
    {
        if(*pCur == '\"')
            break;
        pCur++;
    }
}
inline void HTMLParser::parseQuotation(char*& pCur,char* pEnd)
{
    pCur++;
    while (pCur < pEnd)
    {
        if(*pCur == '\'')
            break;
        pCur++;
    }
}
inline void HTMLParser::parseScript(char*& pCur,char* pEnd)
{
    pCur += 7; ///skip "script "
    htmlelem_t eCurToken = ET_SCRIPT_START;
    while (pCur < pEnd)
    {
        switch(*pCur)
        {
        case '\"':
            parseDQuotation(pCur,pEnd);
            break;
        case '\'':
            parseQuotation(pCur,pEnd);
            break;
        case '>':
            eCurToken = ET_SCRIPT_END;
            break;
        case '<':
            eCurToken = ET_SCRIPT_START;
            if( (pCur + 1 < pEnd) && *(pCur + 1) == '/')
            {
                pCur++;
                while (pCur < pEnd)
                {
                    if(*pCur == '>')
                        break;
                    pCur++;
                }
                pCur++;
                return;
            }

            break;
        case '/':
        {
            if(pCur + 1 < pEnd)
            {
                if(*(pCur + 1) == '/')///skip comment:"//..."
                {
                    while (pCur < pEnd)
                    {
                        if(*pCur == '\r' || *pCur == '\n')
                            break;
                        pCur++;
                    }
                }
                else if(*(pCur + 1) == '*')///skip comment:"/* ... */"
                {
                    pCur += 3;
                    while (pCur < pEnd)
                    {
                        if(*(pCur - 1) == '*' || *pCur == '/')
                            break;
                        pCur++;
                    }
                }
                else if((eCurToken == ET_SCRIPT_START) && *(pCur + 1) == '>')///"/>"
                {
                    pCur += 2;
                    return ;
                }
            }
        }
        break;
        }
        pCur++;
    }
}
inline void HTMLParser::parseStyle(char*& pCur,char* pEnd)
{
    pCur += 6; ///skip "style "
    while (pCur < pEnd)
    {
        switch(*pCur)
        {
        case '\"':
            parseDQuotation(pCur,pEnd);
            break;
        case '\'':
            parseQuotation(pCur,pEnd);
            break;
        case '<':					
            if( (pCur + 1 < pEnd) && *(pCur + 1) == '/')
            {
                pCur++;
                while (pCur < pEnd)
                {
                    if(*pCur == '>')
                        break;
                    pCur++;
                }
                pCur++;
                return;
            }

            break;
        case '/':
            if( (pCur + 1 < pEnd) && *(pCur + 1) == '>')
            {
                pCur += 2;
                return;
            }   
            break;
        }
        pCur++;
    }
}
inline void HTMLParser::parseCData(char*& pCur,char* pEnd)
{
    ///"<![CDATA[" and "]]>"
    pCur += 8;///"![CDATA["

    while (pCur < pEnd)
    {
        if( (*pCur == '>') && (*(pCur - 1) == ']') &&(*(pCur - 2) == ']') )
            break;
        pCur++;
    }
}
inline void HTMLParser::parsePI(char*& pCur,char* pEnd)
{
    ///"<?" and "?>"
    pCur++;///"?"

    while (pCur < pEnd)
    {
        if( (*pCur == '>') && (*(pCur - 1) == '?'))
            break;
        pCur++;
    }
}
inline void HTMLParser::parseComment(char*& pCur,char* pEnd)
{
    ///"<!--" and "-->"
    pCur += 3;///"!--"

    while (pCur < pEnd)
    {
        if( (*pCur == '>') && (*(pCur - 1) == '-') && (*(pCur - 2) == '-'))
            break;
        pCur++;
    }
}
inline void HTMLParser::parseEntityDecl(char*& pCur,char* pEnd)
{
    ///""<!ENTITY" and ">"
    pCur += 7;///"!ENTITY"

    while (pCur < pEnd)
    {
        if(*pCur == '>')
            break;
        pCur++;
    }
}
inline char HTMLParser::parseEntity(char*& pCur,char* pEnd)
{
    pCur++; ///skip"&"
    char* pEntity = pCur;
    char* pEntityEnd = pCur;

    int i = 0;
    for (; i < 31 && pEntityEnd < pEnd; ++i)
    {
        if(*pEntityEnd == ';')
            break;
        pEntityEnd++;
    }
    pCur = pEntityEnd + 1;
    if(i == 2)  
    {
        if(_strncmp(pEntity,"gt",2)) 
        {
            return '>';
        }
        if(_strncmp(pEntity,"lt",2))
        {
            return '<';
        }
    }
    else if((i == 3) && _strncmp(pEntity,"amp",3)) 
    {
        return '&';
    }
    else if(i == 4) 
    {
        if(_strncmp(pEntity,"apos",4))
            return '\'';
        if(_strncmp(pEntity,"quot",4)) 
            return '\"';
        if(_strncmp(pEntity,"nbsp",4)) 
            return ' ';
    }
    /*else if( (*pEntity == '#') && (pEntity < pEnd) && isDigit(*(pEntity + 1)))
      {
      pEntity++;
      int nChar;
      if(sscanf(pEntity,"%lu;",&nChar) == 1)
      {
      return (char)nChar;
      }
      }*/
    pCur = pEntity;
    return '&';
}

FX_NS_END

#endif //

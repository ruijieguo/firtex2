#include <stdio.h>
#include <string.h>
#include "MSOfficeTextStripper.h"

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;
FX_NS_DEF(plugin);


MSOfficeTextStripper::MSOfficeTextStripper(void)
    : m_pTextBuffer(NULL)
    , m_nTextPos(0)
    , m_nTextSize(0)
{
    m_pAnalyze = new Analyze(this);
}

MSOfficeTextStripper::~MSOfficeTextStripper(void)
{
    if (m_pTextBuffer)
    {
        delete m_pTextBuffer;
        m_pTextBuffer = NULL;
    }
    m_nTextSize = 0;
    m_nTextPos = 0;
    delete m_pAnalyze;
}

char* MSOfficeTextStripper::strip(const char* szFileName)
{
    m_nTextPos = 0;

    FILE* hFile = fopen(szFileName, "rb");
    if (!hFile) 
        return NULL;

    const char* p = szFileName;
    while(*p)
        p++;
    p--;
    if (*p == L't')
        m_pAnalyze->parsePPT(hFile);
    else if (*p == L's')
        m_pAnalyze->parseXLS(hFile);
    else
        m_pAnalyze->parseDOC(hFile);

    fclose(hFile);

    if (m_nTextPos > 0 )
        return m_pTextBuffer;
    return NULL;
}

void MSOfficeTextStripper::output_paragraph(unsigned short int *buffer)
{
    size_t len = wcslen((const wchar_t*)buffer);
    if (!m_pTextBuffer || (m_nTextSize - m_nTextPos < len) )
        growBuffer(0);
#ifdef WIN32
    int nRet = WideCharToMultiByte(CP_ACP, 0, (const wchar_t*)buffer, (int)len, 
                                   m_pTextBuffer + m_nTextPos, (int)m_nTextSize - m_nTextPos, NULL, NULL);
    if (nRet > 0)
    {
        m_nTextPos += nRet;	
    }
#else	
    mbstate_t state;
    memset( &state, 0, sizeof(state) );
    wchar_t const* tmp = (wchar_t const*)buffer;
    size_t nRet = wcsrtombs(m_pTextBuffer + m_nTextPos,&tmp,m_nTextSize - m_nTextPos,&state);	
    if (nRet != (size_t)-1)
        m_nTextPos += nRet;
#endif
    if (m_nTextSize - m_nTextPos > 0)
        m_pTextBuffer[m_nTextPos++] = ' ';
}

void MSOfficeTextStripper::growBuffer(size_t nGrowLen)
{
    size_t nNewLen = m_nTextSize + nGrowLen;
    if (nNewLen < (m_nTextSize << 1) )
        nNewLen = (m_nTextSize << 1);
    if (nNewLen < (size_t)DEFAULT_TEXTBUF_SIZE)
        nNewLen = DEFAULT_TEXTBUF_SIZE;
    char* pBuf = new char[nNewLen];
    if (m_nTextPos > 0)
        memcpy(pBuf,m_pTextBuffer,m_nTextPos);
    delete[] m_pTextBuffer;
    m_pTextBuffer = pBuf;
    m_nTextSize = nNewLen;
}

FX_NS_END

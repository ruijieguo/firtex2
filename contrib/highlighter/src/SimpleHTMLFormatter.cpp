#include "firtex/contrib/highlighter/SimpleHTMLFormatter.h"

using namespace std;

FX_NS_DEF(highlight);

SimpleHTMLFormatter::SimpleHTMLFormatter(const string& szPreTag, const string& szPostTag)
	: m_sPreTag(szPreTag)
	, m_sPostTag(szPostTag)
{			
}

SimpleHTMLFormatter::SimpleHTMLFormatter()
	: m_sPreTag("<B>")
	, m_sPostTag("</B>")
{			
}

SimpleHTMLFormatter::~SimpleHTMLFormatter(void)
{
}

void SimpleHTMLFormatter::highlightTerm(string& sMarkedText,
                                        const char* szOriginalText, 
                                        int32_t nStartPos, int32_t nEndPos)
{
    sMarkedText.append(m_sPreTag);
    sMarkedText.append(szOriginalText + nStartPos, nEndPos - nStartPos);
    sMarkedText.append(m_sPostTag);
}

void SimpleHTMLFormatter::setTags(const string& sPreTag,const string& sPostTag)
{
    m_sPreTag = sPreTag;
    m_sPostTag = sPostTag;
}

FX_NS_END


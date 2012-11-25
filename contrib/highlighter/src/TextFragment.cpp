#include "firtex/contrib/highlighter/TextFragment.h"
#include "firtex/contrib/highlighter/Formatter.h"
#include "firtex/contrib/highlighter/Encoder.h"

using namespace std;

FX_NS_DEF(highlight);

TextFragment::TextFragment(int32_t nTextStartPos, int32_t nFragNum)
    : m_nTextStartPos(nTextStartPos)
    , m_nTextEndPos(nTextStartPos)
    , m_nFragNum(nFragNum)
    , m_fScore(0.0f)
{
}

TextFragment::~TextFragment(void)
{
}

void TextFragment::setScore(float fScore)
{
    m_fScore = fScore;
}

float TextFragment::getScore() const
{
    return m_fScore;
}

void TextFragment::merge(const TextFragment* pFrag2)
{
    m_nTextEndPos = pFrag2->m_nTextEndPos;
    m_matchedOffsets.insert(m_matchedOffsets.end(),
                            pFrag2->m_matchedOffsets.begin(),
                            pFrag2->m_matchedOffsets.end());
    m_fScore = max(m_fScore, pFrag2->m_fScore);
}

bool TextFragment::follows(const TextFragment* pFragment)
{
    return m_nTextStartPos == pFragment->m_nTextEndPos;
}

int32_t TextFragment::getFragNum() const
{
    return m_nFragNum;
}

void TextFragment::markString(std::string& sMarkedText, const char* szText,
                              size_t nTextLen, Formatter* pFormatter, Encoder* pEncoder)
{
    size_t nTextStartPos = m_nTextStartPos;
    size_t nTextEndPos = (m_matchedOffsets.size() > 0) 
                         ? m_matchedOffsets[0].first : m_nTextEndPos;
    
    pEncoder->encodeText(szText + nTextStartPos, 
                         nTextEndPos - nTextStartPos, sMarkedText);
    
    for (size_t i = 0; i < m_matchedOffsets.size(); ++i)
    {
        nTextStartPos = m_matchedOffsets[i].first;
        nTextEndPos = m_matchedOffsets[i].second;
        pFormatter->highlightTerm(sMarkedText, szText, (int32_t)nTextStartPos,
                m_matchedOffsets[i].second);
        nTextStartPos = nTextEndPos;

        if ((i + 1)< m_matchedOffsets.size())
        {
            nTextEndPos = m_matchedOffsets[i + 1].first;
        }
        if (nTextEndPos > nTextStartPos)
        {
            pEncoder->encodeText(szText + nTextStartPos, 
                    nTextEndPos - nTextStartPos, sMarkedText);
        }
    }
    if (m_matchedOffsets.size() > 0 
        && (m_matchedOffsets.rbegin()->second != m_nTextEndPos))
    {
        nTextStartPos = m_matchedOffsets.rbegin()->second;
        pEncoder->encodeText(szText + nTextStartPos,
                             m_nTextEndPos - nTextStartPos, 
                             sMarkedText); 
    }
}

FX_NS_END

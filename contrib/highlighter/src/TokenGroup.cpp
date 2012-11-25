#include "firtex/contrib/highlighter/TokenGroup.h"
#include "firtex/analyzer/Token.h"

#include <math.h>


FX_NS_USE(analyzer);

FX_NS_DEF(highlight);

TokenGroup::TokenGroup()
    : m_nNumTokenView(0)
    , m_nStartOffset(0)
    , m_nEndOffset(0)
    , m_fTot(0.0)
    , m_nMatchStartOffset(0)
    , m_nMatchEndOffset(0)
{
}

TokenGroup::~TokenGroup()
{
    clear();
}

void TokenGroup::addToken(const Token* pToken, float score)
{
    if (m_nNumTokenView < MAX_NUM_TOKENS_PER_GROUP)
    {	    
        if (m_nNumTokenView == 0)
        {
            m_nStartOffset = m_nMatchStartOffset = pToken->getStartOffset();
            m_nEndOffset = m_nMatchEndOffset = pToken->getEndOffset();
            m_fTot += score;
        }
        else
        {
            m_nStartOffset = min(m_nStartOffset, pToken->getStartOffset());
            m_nEndOffset = max(m_nEndOffset, pToken->getEndOffset());
            if (score > 0) 
            {
                if (m_fTot == 0)
                {
                    m_nMatchStartOffset = pToken->getStartOffset();
                    m_nMatchEndOffset = pToken->getEndOffset();
                }
                else 
                {
                    m_nMatchStartOffset = min(m_nMatchStartOffset, pToken->getStartOffset());
                    m_nMatchEndOffset = max(m_nMatchEndOffset, pToken->getEndOffset());
                }
                m_fTot += score;
            }
        }
        m_nNumTokenView++;
    }
}

bool TokenGroup::isDistinct(const Token* pToken) const
{
    return pToken->getStartOffset() >= m_nEndOffset;
}

void TokenGroup::clear()
{
    m_nStartOffset = m_nMatchStartOffset = 0;
    m_nEndOffset = m_nMatchEndOffset = 0;
    m_nNumTokenView = 0;
    m_fTot = 0;
}

FX_NS_END

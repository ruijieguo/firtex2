#include "firtex/analyzer/TokenFilter.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(utility);

FX_NS_DEF(analyzer);

std::string TokenFilter::PARAM_SEPERATOR = ",";
std::string TokenFilter::EQUAL_MARK = "=";

TokenFilter::TokenFilter(void)
    : m_pPreFilter(NULL)
{
}

TokenFilter::TokenFilter(const TokenFilter& src)
    : m_pPreFilter(NULL)
{
    if (src.m_pPreFilter)
    {
        m_pPreFilter = src.m_pPreFilter->clone();
    }
}

TokenFilter::TokenFilter(TokenFilter* pPreFilter)
    : m_pPreFilter(pPreFilter)
{
}

TokenFilter::~TokenFilter(void)
{
    if(m_pPreFilter)
    {
        delete m_pPreFilter;
        m_pPreFilter = NULL;
    }
}

void TokenFilter::filter(TokenSourcePtr& tokenSource) const
{
    if(m_pPreFilter)
    {
        m_pPreFilter->filter(tokenSource);
    }
    filterInternal(tokenSource);
}

void TokenFilter::attachFilter(TokenFilter* pPreFilter)
{
    if(m_pPreFilter)
    {
        m_pPreFilter->attachFilter(pPreFilter);
    }
    else 
    {
        m_pPreFilter = pPreFilter;
    }
}

TokenFilter* TokenFilter::detachFilter(TokenFilter* pPreFilter)
{
    if(m_pPreFilter)
    {
        TokenFilter* pPreFilter = m_pPreFilter;
        m_pPreFilter = NULL;
        return pPreFilter;
    }
    return NULL;
}

TokenFilter* TokenFilter::detachFilter(const tstring& szIdentifier)
{
    TokenFilter* pPreFilter = m_pPreFilter;
    if(pPreFilter)
    {
        if(startsWithNoCase(pPreFilter->getIdentifier(), szIdentifier))
        {
            m_pPreFilter = NULL;
            return pPreFilter;
        }
    }
    return NULL;
}

FX_NS_END

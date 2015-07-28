#include "firtex/analyzer/TokenSource.h"

FX_NS_DEF(analyzer)

TokenSource::TokenSource(void)
{
}
 
TokenSource::~TokenSource(void)
{
}

void TokenSource::setOriginalView(const char* szText, size_t length)
{
    if (!m_pOrgView)
    {
        if (length > DEFAULT_ORIGINAL_TEXT_SIZE)
        {
            m_pOrgView.reset(new TokenView(length + 1));
        }
        else 
        {
            m_pOrgView.reset(new TokenView(DEFAULT_ORIGINAL_TEXT_SIZE));
        }
    }
    else 
    {
        m_pOrgView->clear();
    }

    if (!m_pLastView)
    {
        m_pLastView = m_pOrgView;
    }

    m_pOrgView->addToken(szText, length, 1, 0, (int32_t)length);
}

void TokenSource::setOriginalView(const TokenViewPtr& pTokenView)
{
    m_pOrgView = pTokenView;
    if (!m_pLastView)
    {
        m_pLastView = pTokenView;
    }
}

void TokenSource::addCustomView(const tstring& sViewName,
                                const TokenViewPtr& pTokenView)
{
    ViewIterator it = m_customViewMap.find(sViewName);
    if (it == m_customViewMap.end())
    {
        m_customViewMap.insert(make_pair(sViewName, pTokenView));
        m_pLastView = pTokenView;
    }
}

TokenViewPtr TokenSource::acquireCustomView(const tstring& sViewName)
{
    ViewIterator it = m_customViewMap.find(sViewName);
    if (it != m_customViewMap.end())
    {
        m_pLastView = it->second;
        return m_pLastView;
    }
    else
    {
        TokenViewPtr pTokenView(new TokenView());
        addCustomView(sViewName, pTokenView);
        return pTokenView;
    }
}

void TokenSource::reset()
{
    if (m_pOrgView)
    {
        m_pOrgView->clear();
    }
    m_pLastView.reset();

    for (ViewMap::iterator it = m_customViewMap.begin();
         it != m_customViewMap.end(); ++it)
    {
        it->second->clear();
    }
}

FX_NS_END

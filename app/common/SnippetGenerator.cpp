#include "SnippetGenerator.h"
#include "firtex/contrib/highlighter/Highlighter.h"
#include "firtex/contrib/highlighter/QueryScorer.h"
#include "firtex/contrib/highlighter/SimpleHTMLFormatter.h"

using namespace std;

FX_NS_USE(search);
FX_NS_USE(analyzer);
FX_NS_USE(highlight);

FX_NS_DEF(app);

SETUP_LOGGER(app, SnippetGenerator);

bool SnippetGenerator::filter(const string& sOrgValue, 
                              string& sNewValue) const
{

    QueryScorer* pQueryScorer = new QueryScorer(m_pQuery.get(), m_sField.c_str());
    HighlighterPtr pHigh = new Highlighter(new SimpleHTMLFormatter(
                    m_sPreTag, m_sPostTag), pQueryScorer);
    pHigh->getBestFragments(sNewValue, m_pAnalyzer, sOrgValue.c_str(),
                            sOrgValue.length(), 2, m_sSeparator);
    return true;
}

FX_NS_END

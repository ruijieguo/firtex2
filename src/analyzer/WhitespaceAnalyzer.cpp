#include "firtex/analyzer/WhitespaceAnalyzer.h"
#include "firtex/utility/StringTokenizer.h"

FX_NS_USE(utility);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, WhitespaceAnalyzer);

WhitespaceAnalyzer::WhitespaceAnalyzer() 
{
    setMaxTokensToAnalyze(MAX_TOKEN_TO_ANALYZE);
}

WhitespaceAnalyzer::WhitespaceAnalyzer(const WhitespaceAnalyzer& src) 
    : Analyzer(src)
{
}

WhitespaceAnalyzer::~WhitespaceAnalyzer() 
{
}

const tstring WhitespaceAnalyzer::getIdentifier() const 
{
    return WhitespaceAnalyzer::Creator::instance()->getIdentifier();
};

Analyzer* WhitespaceAnalyzer::clone() const
{
    return new WhitespaceAnalyzer(*this);
}

void WhitespaceAnalyzer::doTokenize(TokenSourcePtr& tokenSource) const
{
    TokenViewPtr pInputTokens = tokenSource->getLastView();
    TokenViewPtr pOutputTokens = tokenSource->acquireCustomView(_T("whitespace_view"));
    pOutputTokens->reserve(getMaxTokensToAnalyze());
    pOutputTokens->setTokenType(Token::TT_STRING);

    TokenView::Iterator it = pInputTokens->iterator();
    while (it.hasNext())
    {
        const Token& token = it.next();

        const char* pText = token.getTextValue();
        size_t length = token.getTextLength();
        
        StringTokenizer st;
        st.tokenize(pText, length, " ", StringTokenizer::TOKEN_TRIM
                    | StringTokenizer::TOKEN_IGNORE_EMPTY);
        for (StringTokenizer::Iterator it = st.begin(); it != st.end(); ++it)
        {
            const std::string& str = *it;
            pOutputTokens->addToken(str.c_str(), str.length());
        }
    }
}

FX_NS_END


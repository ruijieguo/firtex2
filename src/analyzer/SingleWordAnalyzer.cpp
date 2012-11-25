#include "firtex/analyzer/SingleWordAnalyzer.h"
#include "firtex/analyzer/StandardTokenizer.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/KeyValueParser.h"

FX_NS_USE(utility);

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, SingleWordAnalyzer);

tstring SingleWordAnalyzer::PARAM_ENCODE = _T("encode");

SingleWordAnalyzer::SingleWordAnalyzer() 
    : m_sEncodeName("utf-8")
{
}

SingleWordAnalyzer::SingleWordAnalyzer(const tstring& sEncode)
    : m_sEncodeName(sEncode)
{
}

SingleWordAnalyzer::SingleWordAnalyzer(const SingleWordAnalyzer& src)
    : m_sEncodeName(src.m_sEncodeName)
    , m_pTokenizer(src.m_pTokenizer)
{
}

SingleWordAnalyzer::~SingleWordAnalyzer() 
{
}

void SingleWordAnalyzer::init()
{
    if (m_sEncodeName == "gbk" || m_sEncodeName == "gb2312")
    {
        m_pTokenizer = new GBKStandardTokenizer();
    }
    else
    {
        m_pTokenizer = new UTF8StandardTokenizer();
    }
}

void SingleWordAnalyzer::doTokenize(TokenSourcePtr& tokenSource) const
{
    FIRTEX_ASSERT2(m_pTokenizer != NULL);

    TokenViewPtr pInTokenView = tokenSource->getLastView();
    TokenViewPtr pOutTokenView = tokenSource->acquireCustomView(_T("single_view"));
    pOutTokenView->reserve(getMaxTokensToAnalyze());
    pOutTokenView->setTokenType(Token::TT_CWORD);
     
    TokenView::Iterator iter = pInTokenView->iterator();
    while(iter.hasNext())
    {
        const Token& token = iter.next();
        m_pTokenizer->tokenize(pOutTokenView,
                              token.getTextValue(),
                              token.getTextLength());
    }
}

void SingleWordAnalyzer::setParam(const tstring& sParam)
{
    KeyValueParser kvParser;
    bool ret = kvParser.parse(sParam, _T(","), _T("="));
    if (!ret)
    {
        FX_LOG(ERROR, _T("Invalid analyzer parameter: [%s]"),
               sParam.c_str());
    }
    else
    {
        tstring sValue;
        if (kvParser.getValue(PARAM_ENCODE, sValue))
        {
            if ((!strCompareNoCase(sValue.c_str(), "utf-8"))
                || (!strCompareNoCase(sValue.c_str(), "gbk")) 
                || (!strCompareNoCase(sValue.c_str(), "gb2312")))
            {
                m_sEncodeName = toLower(sValue);
            }
            else
            {
                FX_LOG(ERROR, _T("Invalid parameter: [encode=%s], "
                                "using default [encode=utf-8]"),
                       sValue.c_str());
                m_sEncodeName = toLower(sValue);
            }
        }
    }
}

const tstring SingleWordAnalyzer::getIdentifier() const 
{
    return SingleWordAnalyzer::Creator::instance()->getIdentifier();
}

Analyzer* SingleWordAnalyzer::clone() const
{
    return new SingleWordAnalyzer(*this);
}


FX_NS_END


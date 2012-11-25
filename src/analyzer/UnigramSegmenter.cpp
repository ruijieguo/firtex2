#include <stack>
#include "firtex/analyzer/UnigramSegmenter.h"

using namespace std;

FX_NS_DEF(analyzer);

double UnigramSegmenter::DEFAULT_LAMBDA = 0.5;

SETUP_STREAM_LOGGER(analyzer, UnigramSegmenter);

UnigramSegmenter::UnigramSegmenter(
        const TokenizerPtr& pUnknownWordTokenizer) 
    : m_pUnknownWordTokenizer(pUnknownWordTokenizer)
    , m_dbLambda(DEFAULT_LAMBDA)
    , m_nMaxTokenLenght(DEFAULT_MAX_TOKEN_LENGTH)
{
}

UnigramSegmenter::UnigramSegmenter(const UnigramSegmenter& src)
    : m_pLexicon(src.m_pLexicon)
    , m_pUnknownWordTokenizer(src.m_pUnknownWordTokenizer)
    , m_dbLambda(src.m_dbLambda)
    , m_nMaxTokenLenght(src.m_nMaxTokenLenght)
{
}

UnigramSegmenter::~UnigramSegmenter() 
{
}

void UnigramSegmenter::init(const tstring& sLexiconFile)
{
    m_pLexicon = new DoubleArrayTrie();
    m_pLexicon->load(sLexiconFile);
}

void UnigramSegmenter::segment(TokenViewPtr& pTokenView, 
                               const char* szText, size_t length) const
{
    uint64_t nNumTerms = m_pLexicon->getSumValue();
    uint32_t nNumTypes = m_pLexicon->getInsertCount();

    TokenBuffer buffer;
    
    int32_t nPosInc = 0;
    const char* pTextStart = szText;
    size_t nCurPos = 0;

    Tokenizer::LexerWrapper* pLexer = 
        m_pUnknownWordTokenizer->createLexer(szText, length);

    do 
    {
        ///Segment sentence to atomic token
        const char* szToken = NULL;
        size_t nTokenLen = 0;
        size_t nTokenPos = 0;
        size_t nTokenEndPos = 0;
        double lp;
        Token::TokenType tokenType;

        while ((tokenType = pLexer->lex(szToken, nTokenLen)) != Token::TT_MAX)
        {
            nTokenEndPos = nTokenPos + nTokenLen;
            if (nTokenEndPos >= buffer.nBufSize)
            {
                buffer.resize(length + 1);
            }
            
            buffer.pAtomicBackrefBuf[nTokenEndPos] = (int32_t)nTokenPos;
            buffer.pTypeBuf[nTokenPos] = tokenType;

            if (szToken)
            {
                lp = estimateScore(1, nNumTerms, nNumTypes);
                buffer.pScoreBuf[nTokenEndPos] = buffer.pScoreBuf[nTokenPos] + lp;
                buffer.pBackrefBuf[nTokenEndPos] = (int32_t)nTokenPos;
                nTokenPos = nTokenEndPos;
            }
            else
            {
                buffer.pScoreBuf[nTokenEndPos] = 0;
                buffer.pBackrefBuf[nTokenEndPos] = (int32_t)-1;
                nTokenPos = nTokenEndPos;
                break;
            }
        }//while

        buffer.pScoreBuf[0] = 0;

        /// Calculate score using Dynamic Programming(DP)
        for (size_t i = 1; i <= nTokenEndPos; i++) 
        {
            if (buffer.pAtomicBackrefBuf[i] == -1)
            {
                continue;
            }

            for (int32_t j = (int32_t)buffer.pAtomicBackrefBuf[i]; j > 0; j = buffer.pAtomicBackrefBuf[j])
            {
                if (buffer.pAtomicBackrefBuf[j] != -1)
                {
                    if (i - buffer.pAtomicBackrefBuf[j] > m_nMaxTokenLenght)
                    {
                        break;
                    }

                    int v = m_pLexicon->search(pTextStart + buffer.pAtomicBackrefBuf[j],
                            i - buffer.pAtomicBackrefBuf[j]);
                    if (v > 0)
                    {
                        lp = estimateScore(v, nNumTerms, nNumTypes);
                        if (buffer.pScoreBuf[i] < buffer.pScoreBuf[buffer.pBackrefBuf[j]] + lp)
                        {
                            buffer.pScoreBuf[i] = buffer.pScoreBuf[buffer.pBackrefBuf[j]] + lp;
                            buffer.pBackrefBuf[i] = buffer.pAtomicBackrefBuf[j];
                        }
                    }
                }
            }
        }

        typedef std::pair<size_t, size_t> TokenStr;
        typedef std::vector<TokenStr> TokenVect;
        TokenVect revTokens;
        revTokens.reserve(nTokenEndPos);

        for (size_t i = nTokenEndPos; i > 0;) 
        {
            if (buffer.pBackrefBuf[i] != -1)
            {
                revTokens.push_back(make_pair((size_t)buffer.pBackrefBuf[i],
                                (size_t)(i - buffer.pBackrefBuf[i])));
                i = buffer.pBackrefBuf[i];
            }
            else 
            {
                i = buffer.pAtomicBackrefBuf[i];
            }
        }

        size_t nStartOff = 0;
        size_t nLength = 0;
//        size_t nPrevEndOff = 0;
        for (TokenVect::const_reverse_iterator it = revTokens.rbegin();
             it != revTokens.rend(); ++it)
        {
            const TokenStr& tokenStr = *it;
            nStartOff = tokenStr.first + nCurPos;
            nLength = tokenStr.second;
            pTokenView->addToken(szText + nStartOff, nLength, nPosInc,
                    (int32_t)nStartOff, (int32_t)(nStartOff + nLength));

            nPosInc = 1;
//            nPrevEndOff = nStartOff + nLength;
        }

        pTextStart += nTokenPos;
        nCurPos += nTokenPos;

        if (buffer.pTypeBuf[nTokenEndPos - nTokenLen] != Token::TT_WHITESPACE)
        {
            ++nPosInc;
        }
    } while(nCurPos < length);

    delete pLexer;
}

Segmenter* UnigramSegmenter::clone() const
{
    return new UnigramSegmenter(*this);   
}

FX_NS_END


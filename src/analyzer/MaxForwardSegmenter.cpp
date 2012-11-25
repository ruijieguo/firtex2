#include "firtex/analyzer/MaxForwardSegmenter.h"

FX_NS_DEF(analyzer);

SETUP_STREAM_LOGGER(analyzer, MaxForwardSegmenter);

MaxForwardSegmenter::MaxForwardSegmenter(
        const TokenizerPtr& pUnknownWordTokenizer) 
    : m_pSingleWordTokenizer(pUnknownWordTokenizer)
{
}

MaxForwardSegmenter::MaxForwardSegmenter(const MaxForwardSegmenter& src)
    : m_pLexicon(src.m_pLexicon)
    , m_pSingleWordTokenizer(src.m_pSingleWordTokenizer)
{
}

MaxForwardSegmenter::~MaxForwardSegmenter() 
{
}

void MaxForwardSegmenter::init(const tstring& sLexiconFile)
{
    m_pLexicon = new DoubleArrayTrie();
    m_pLexicon->load(sLexiconFile);
}

void MaxForwardSegmenter::segment(TokenViewPtr& pTokenView, 
                                  const char* szText, size_t length) const
{
    TokenBuffer buffer;

    int hintId = 0;
    size_t nMatchLen = 0;

    int32_t nPosInc = 0;
    const char* pTextStart = szText;
    const char* pMatchedEnd = NULL;

    size_t nCurPos = 0;

    Tokenizer::LexerWrapper* pLexer = 
        m_pSingleWordTokenizer->createLexer(szText, length);

    do 
    {
        ///Segment sentence to atomic token
        const char* szToken = NULL;
        size_t nTokenLen = 0;
        size_t nTokenPos = 0;
        size_t nTokenEndPos = 0;
        Token::TokenType tokenType;

        while ((tokenType = pLexer->lex(szToken, nTokenLen)) != Token::TT_MAX)
        {
            nTokenEndPos = nTokenPos + nTokenLen;
            if (nTokenEndPos >= buffer.nBufSize)
            {
                buffer.resize(length + 1);
            }
            
            buffer.pTypeBuf[nTokenPos] = tokenType;

            if (szToken)
            {
                buffer.pForwardRefBuf[nTokenPos] = (int32_t)nTokenEndPos;
            }
            else
            {
                buffer.pForwardRefBuf[nTokenPos] = -(int32_t)nTokenEndPos;
                break;
            }
            nTokenPos = nTokenEndPos;
        }// end while

        for (size_t i = 0; i < nTokenEndPos;) 
        {
            if (buffer.pForwardRefBuf[i] < 0)
            {
                i = -(buffer.pForwardRefBuf[i]);
                continue;
            }

            hintId = m_pLexicon->search(pTextStart + i,
                    length - nCurPos - i, &pMatchedEnd);
            if (hintId != 0)
            {
                nMatchLen = (size_t)(pMatchedEnd - pTextStart - i);
                pTokenView->addToken(pTextStart + i, nMatchLen,
                        nPosInc, (int32_t)(nCurPos + i), (int32_t)(nCurPos + i + nMatchLen));
            }
            else
            {
                nMatchLen = buffer.pForwardRefBuf[i] - i;
                pTokenView->addToken(pTextStart + i, nMatchLen,
                        nPosInc, (int32_t)(nCurPos + i), (int32_t)(nCurPos + i + nMatchLen));
            }

            i += nMatchLen;
            nPosInc = 1;
        }

        pTextStart += nTokenEndPos;
        nCurPos += nTokenEndPos;

        if (buffer.pTypeBuf[nTokenEndPos - nTokenLen] != Token::TT_WHITESPACE)
        {
            ++nPosInc;
        }
    } while(nCurPos < length);

    delete pLexer;
}

Segmenter* MaxForwardSegmenter::clone() const
{
    return new MaxForwardSegmenter(*this);   
}

FX_NS_END


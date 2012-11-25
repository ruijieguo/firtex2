//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : Ruijie Guo
// Email        : ruijieguo@gmail.com
// Created      : 2011-03-23 23:28:29

#ifndef __FX_UNIGRAMSEGMENTER_H
#define __FX_UNIGRAMSEGMENTER_H

#include <math.h>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/analyzer/DoubleArrayTrie.h"
#include "firtex/analyzer/StandardTokenizer.h"
#include "firtex/analyzer/Segmenter.h"

FX_NS_DEF(analyzer);

class UnigramSegmenter : public Segmenter
{
public:
    static double DEFAULT_LAMBDA;
    const static size_t DEFAULT_MAX_TOKEN_LENGTH = 16;

public:
    UnigramSegmenter(const TokenizerPtr& pUnknownWordTokenizer);
    UnigramSegmenter(const UnigramSegmenter& src);
    ~UnigramSegmenter();

public:
    /**
     * Load lexicon and initialize
     * @param sLexiconFile file path of lexicon
     */
    void init(const tstring& sLexiconFile);

    /**
     * Segment text to tokens
     * @param pTokenView result segmented tokens
     * @param szText text need to segment
     * @param length of text in bytes
     */
    void segment(TokenViewPtr& pTokenView, 
                 const char* szText, size_t length) const;

    /**
     * Clone a new segmenter
     */
    Segmenter* clone() const;

private:
    void segmentUnknownWord(TokenViewPtr& pTokenView,
                            std::string& sUnknownWords) const;

    double estimateScore(int v, uint64_t n, uint32_t t) const;

protected:
    DoubleArrayTriePtr m_pLexicon;
    TokenizerPtr m_pUnknownWordTokenizer;
    double m_dbLambda;
    size_t m_nMaxTokenLenght;

private:
    struct TokenBuffer
    {
        const static size_t DEFAULT_BUFFER_SIZE = 128;

        TokenBuffer()
            : pScoreBuf((double*)scoreBuf)
            , pBackrefBuf((int32_t*)backrefBuf)
            , pAtomicBackrefBuf((int32_t*)atomicBackrefBuf)
            , pTypeBuf((Token::TokenType*)typeBuf)
            , nBufSize(DEFAULT_BUFFER_SIZE)
        {
            for (size_t i = 0; i < DEFAULT_BUFFER_SIZE; ++i)
            {
                scoreBuf[i] = 0;
                atomicBackrefBuf[i] = backrefBuf[i] = -1;
                typeBuf[i] = Token::TT_UNKNOWN;
            }
        }

        ~TokenBuffer()
        {
            if (nBufSize != DEFAULT_BUFFER_SIZE)
            {
                delete[] pScoreBuf;
                delete[] pBackrefBuf;
                delete[] pAtomicBackrefBuf;
                delete[] pTypeBuf;
            }
        }

        void resize(size_t nNewSize)
        {
            assert(nBufSize == DEFAULT_BUFFER_SIZE);
            //resize buffers
            pScoreBuf = new double[nNewSize];
            pBackrefBuf = new int32_t[nNewSize];
            pAtomicBackrefBuf = new int32_t[nNewSize];
            pTypeBuf = new Token::TokenType[nNewSize];

            memcpy(pScoreBuf, scoreBuf, nBufSize * sizeof(double));
            memcpy(pBackrefBuf, backrefBuf, nBufSize * sizeof(int32_t));
            memcpy(pAtomicBackrefBuf, atomicBackrefBuf, nBufSize * sizeof(int32_t));
            memcpy(pTypeBuf, typeBuf, nBufSize * sizeof(Token::TokenType));
            
            for (size_t i = nBufSize; i < nNewSize; i++) 
            {
                pScoreBuf[i] = 0;
                pAtomicBackrefBuf[i] = pBackrefBuf[i] = -1;
                pTypeBuf[i] = Token::TT_UNKNOWN;
            }

            nBufSize = nNewSize;
        }

        double* pScoreBuf;
        int32_t* pBackrefBuf;
        int32_t* pAtomicBackrefBuf;
        Token::TokenType* pTypeBuf;
        size_t nBufSize;
        
    private:
        double scoreBuf[DEFAULT_BUFFER_SIZE];
        int32_t backrefBuf[DEFAULT_BUFFER_SIZE];
        int32_t atomicBackrefBuf[DEFAULT_BUFFER_SIZE];
        Token::TokenType typeBuf[DEFAULT_BUFFER_SIZE];
    };

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////////////
inline void UnigramSegmenter::segmentUnknownWord(TokenViewPtr& pTokenView, 
        std::string& sUnknownWords) const
{
    m_pUnknownWordTokenizer->tokenize(pTokenView, sUnknownWords);
    sUnknownWords.clear();
}

inline double UnigramSegmenter::estimateScore(int v, uint64_t n, uint32_t t) const
{
//    return log(v + m_dbLambda) - log(n + t * m_dbLambda);
    return log(v + m_dbLambda);
}


FX_NS_END

#endif //__FX_UNIGRAMSEGMENTER_H

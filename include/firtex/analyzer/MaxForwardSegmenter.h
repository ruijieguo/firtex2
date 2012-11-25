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
// Created      : 2011-03-19 09:55:11

#ifndef __FX_MAXFORWARDSEGMENTER_H
#define __FX_MAXFORWARDSEGMENTER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/analyzer/Segmenter.h"
#include "firtex/analyzer/DoubleArrayTrie.h"
#include "firtex/analyzer/Tokenizer.h"

FX_NS_DEF(analyzer);

class MaxForwardSegmenter : public Segmenter
{
public:
    MaxForwardSegmenter(const TokenizerPtr& pUnknownWordTokenizer);
    MaxForwardSegmenter(const MaxForwardSegmenter& src);
    ~MaxForwardSegmenter();

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
    DoubleArrayTriePtr m_pLexicon;
    TokenizerPtr m_pSingleWordTokenizer;

private:
    struct TokenBuffer
    {
        const static size_t DEFAULT_BUFFER_SIZE = 128;

        TokenBuffer()
            : pForwardRefBuf((int32_t*)forwardRefBuf)
            , pTypeBuf((Token::TokenType*)typeBuf)
            , nBufSize(DEFAULT_BUFFER_SIZE)
        {
            for (size_t i = 0; i < DEFAULT_BUFFER_SIZE; ++i)
            {
                forwardRefBuf[i] = 0;
                pTypeBuf[i] = Token::TT_UNKNOWN;
            }
        }

        ~TokenBuffer()
        {
            if (nBufSize != DEFAULT_BUFFER_SIZE)
            {
                delete[] pForwardRefBuf;
                delete[] pTypeBuf;
            }
        }

        void resize(size_t nNewSize)
        {
            assert(nBufSize == DEFAULT_BUFFER_SIZE);
            //resize buffers
            pForwardRefBuf = new int32_t[nNewSize];
            pTypeBuf = new Token::TokenType[nNewSize];
                
            memcpy(pForwardRefBuf, forwardRefBuf, nBufSize * sizeof(int32_t));
            memcpy(pTypeBuf, typeBuf, nBufSize * sizeof(Token::TokenType));

            for (size_t i = nBufSize; i < nNewSize; i++) 
            {
                pForwardRefBuf[i] = 0;
                pTypeBuf[i] = Token::TT_UNKNOWN;                
            }

            nBufSize = nNewSize;
        }
        
        int32_t* pForwardRefBuf;
        Token::TokenType* pTypeBuf;
        size_t nBufSize;

    private:
        size_t forwardRefBuf[DEFAULT_BUFFER_SIZE];
        Token::TokenType typeBuf[DEFAULT_BUFFER_SIZE];
    };
    
private:
    DECLARE_STREAM_LOGGER();
};

/////////////////////////////////////////////////
//

FX_NS_END

#endif //__FX_MAXFORWARDSEGMENTER_H

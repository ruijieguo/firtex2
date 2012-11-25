//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-07-13 22:41:24

#ifndef __FX_INMEMPOSPOSTINGDECODER_H
#define __FX_INMEMPOSPOSTINGDECODER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/PosPostingDecoder.h"
#include "firtex/index/PosSkipListReader.h"

FX_NS_DEF(index);

class InMemPosPostingDecoder : public PosPostingDecoder
{
public:
    InMemPosPostingDecoder() {}
    ~InMemPosPostingDecoder() {}

public:
        /**
     * Initialize 
     * 
     * @param termMeta term meta
     * @param pDocStream in-memory input stream for doc-list
     * @param pPosStream in-memory input stream for pos-list
     * @param pDocSkipReader in-memory doc skip list reader 
     * @param pPosSkipReader in-memory pos skip list reader 
     * @param lastDocId the last docid
     * @param lastPos the last position
     */
    void init(const TermMeta& termMeta,
              const FX_NS(store)::InputStreamPtr& pDocStream,
              const FX_NS(store)::InputStreamPtr& pPosStream,
              const DocSkipListReaderPtr& pDocSkipReader,
              const PosSkipListReaderPtr& pPosSkipReader,
              docid_t lastDocId, loc_t lastPos);
};

////////////////////////////////////////////////////////////////
///
inline void InMemPosPostingDecoder::init(const TermMeta& termMeta,
        const FX_NS(store)::InputStreamPtr& pDocStream,
        const FX_NS(store)::InputStreamPtr& pPosStream,
        const DocSkipListReaderPtr& pDocSkipReader,
        const PosSkipListReaderPtr& pPosSkipReader,
        docid_t lastDocId, loc_t lastPos)
{
    m_nNumDocsDecoded = 0;

    m_termMeta = termMeta;
    m_pDocStream = pDocStream;
    m_pDocSkipListReader = pDocSkipReader;

    m_lastPos = lastPos;
    m_pPosStream = pPosStream;
    m_pPosSkipListReader = pPosSkipReader;
    m_lastDocId = lastDocId;
}

FX_NS_END

#endif //__FX_INMEMPOSPOSTINGDECODER_H

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
// Created : 2011-07-13 20:56:16

#ifndef __FX_INMEMDOCPOSTINGDECODER_H
#define __FX_INMEMDOCPOSTINGDECODER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/DocPostingDecoder.h"

FX_NS_DEF(index);

class InMemDocPostingDecoder : public DocPostingDecoder
{
public:
    InMemDocPostingDecoder() {}
    ~InMemDocPostingDecoder() {}

public:
    /**
     * Initialize 
     * 
     * @param termMeta term meta
     * @param pDocStream in-memory input stream for doc-list
     * @param pDocSkipReader p in-memory skip list reader 
     * @param lastDocId the last docid
     */
    void init(const TermMeta& termMeta,
              const FX_NS(store)::InputStreamPtr& pDocStream,
              const DocSkipListReaderPtr& pDocSkipReader,
              docid_t lastDocId);

};

///////////////////////////////////////////////////
//
inline void InMemDocPostingDecoder::init(const TermMeta& termMeta,
        const FX_NS(store)::InputStreamPtr& pDocStream,
        const DocSkipListReaderPtr& pDocSkipReader, 
        docid_t lastDocId)
{
    m_nNumDocsDecoded = 0;

    m_termMeta = termMeta;
    m_pDocStream = pDocStream;
    m_pDocSkipListReader = pDocSkipReader;
    m_lastDocId = lastDocId;
}


FX_NS_END

#endif //__FX_INMEMDOCPOSTINGDECODER_H
